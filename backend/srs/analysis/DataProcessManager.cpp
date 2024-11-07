#include <srs/analysis/DataProcessManager.hpp>
#include <srs/writers/BinaryFileWriter.hpp>
#include <srs/writers/JsonWriter.hpp>
#include <srs/writers/RootFileWriter.hpp>
#include <srs/writers/UDPWriter.hpp>

namespace srs
{
    DataProcessManager::DataProcessManager(DataProcessor* data_processor, asio::thread_pool& thread_pool)
        : struct_deserializer_{ thread_pool }
        , struct_proto_converter_{ thread_pool }
        , proto_deserializer_{ thread_pool }
        , proto_delim_deserializer_{ thread_pool }
        , writers_{ data_processor }
    {
        coro_ = generate_starting_coro(thread_pool.get_executor());
    }

    void DataProcessManager::analysis_one(tbb::concurrent_bounded_queue<SerializableMsgBuffer>& data_queue,
                                          bool is_stopped)
    {
        if (not is_stopped)
        {
            data_queue.pop(binary_data_);
        }
        run_processes(is_stopped);
    }

    void DataProcessManager::run_processes(bool is_stopped)
    {
        auto starting_fut = create_coro_future(coro_, is_stopped).share();
        auto struct_deser_fut = struct_deserializer_.create_future(starting_fut);
        auto proto_converter_fut = struct_proto_converter_.create_future(struct_deser_fut);
        auto proto_deser_fut = proto_deserializer_.create_future(proto_converter_fut);
        auto proto_delim_deser_fut = proto_delim_deserializer_.create_future(proto_converter_fut);

        auto make_writer_future =
            [&starting_fut, &struct_deser_fut, &proto_converter_fut, &proto_deser_fut, &proto_delim_deser_fut](
                auto& writer)
        {
            const auto deser_mode = writer.get_deserialize_mode();
            if constexpr (std::remove_cvref_t<decltype(writer)>::IsStructType)
            {
                return writer.write(struct_deser_fut);
            }
            else
            {
                switch (deser_mode)
                {
                    case raw:
                        return writer.write(starting_fut);
                        break;
                    case proto:
                        return writer.write(proto_deser_fut);
                        break;
                    case proto_frame:
                        return writer.write(proto_delim_deser_fut);
                        break;
                    default:
                        throw std::runtime_error("Unrecognized derserialization option");
                        break;
                }
            }
        };

        writers_.write_with(make_writer_future);
        writers_.wait_for_finished();
    }

    // NOLINTNEXTLINE(readability-static-accessed-through-instance)
    auto DataProcessManager::generate_starting_coro(asio::any_io_executor /*unused*/) -> StartingCoroType
    {
        while (true)
        {
            auto data = std::string_view{ binary_data_.data() };
            auto is_terminated = co_yield (data);
            if (is_terminated)
            {
                co_return;
            }
        }
    }

    void DataProcessManager::reset()
    {
        writers_.reset();
        binary_data_.clear();
    }

    void DataProcessManager::set_output_filenames(const std::vector<std::string>& filenames)
    {
        writers_.set_output_filenames(filenames);
    }
} // namespace srs

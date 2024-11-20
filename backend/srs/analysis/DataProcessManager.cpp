#include <srs/analysis/DataProcessManager.hpp>
#include <srs/writers/BinaryFileWriter.hpp>
#include <srs/writers/JsonWriter.hpp>
#include <srs/writers/RootFileWriter.hpp>
#include <srs/writers/UDPWriter.hpp>

namespace srs
{
    DataProcessManager::DataProcessManager(DataProcessor* data_processor, asio::thread_pool& thread_pool)
        : raw_to_delim_raw_converter_{ thread_pool }
        , struct_deserializer_{ thread_pool }
        , struct_proto_converter_{ thread_pool }
        , proto_serializer_{ thread_pool }
        , proto_delim_serializer_{ thread_pool }
        , writers_{ data_processor }
    {
        coro_ = generate_starting_coro(thread_pool.get_executor());
        coro_sync_start(coro_, false, asio::use_awaitable);
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
        auto raw_to_delim_raw_fut = raw_to_delim_raw_converter_.create_future(starting_fut, writers_);
        auto struct_deser_fut = struct_deserializer_.create_future(starting_fut, writers_);
        auto proto_converter_fut = struct_proto_converter_.create_future(struct_deser_fut, writers_);
        auto proto_deser_fut = proto_serializer_.create_future(proto_converter_fut, writers_);
        auto proto_delim_deser_fut = proto_delim_serializer_.create_future(proto_converter_fut, writers_);

        if (is_stopped)
        {
            spdlog::debug("Shutting down all data writers...");
        }

        auto make_writer_future = [&](auto& writer)
        {
            const auto convert_mode = writer.get_convert_mode();
            if constexpr (std::remove_cvref_t<decltype(writer)>::IsStructType)
            {
                return writer.write(struct_deser_fut);
            }
            else
            {
                switch (convert_mode)
                {
                    case raw:
                        return writer.write(starting_fut);
                        break;
                    case raw_frame:
                        return writer.write(raw_to_delim_raw_fut);
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
        if (is_stopped)
        {
            spdlog::info("All data writers are shutdown.");
        }
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
                spdlog::debug("Shutting down starting coroutine.");
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

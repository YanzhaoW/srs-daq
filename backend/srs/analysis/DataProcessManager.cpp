#include <srs/analysis/DataProcessManager.hpp>

namespace srs
{
    DataProcessManager::DataProcessManager(DataProcessor* data_processor, asio::thread_pool& thread_pool)
        : struct_deserializer_{ thread_pool }
        , struct_proto_converter_{ thread_pool }
        , proto_deserializer_{ thread_pool }
        , writers_{ data_processor }
    {
        coro_ = generate_starting_coro(thread_pool.get_executor());
    }

    void DataProcessManager::analysis_one(auto& data_queue)
    {
        data_queue.pop(binary_data_);

        auto starting_fut = create_coro_future(coro_, false).share();
        auto struct_deser_fut = struct_deserializer_.create_future(starting_fut);
        auto proto_converter_fut = struct_proto_converter_.create_future(struct_deser_fut);
        auto proto_dser_fut = proto_deserializer_.create_future(proto_converter_fut);

        auto make_writer_future =
            [&starting_fut, &struct_deser_fut, &proto_converter_fut, &proto_dser_fut](auto& writer)
        {
            const auto deser_mode = writer.get_deserialize_mode();
            switch (deser_mode)
            {
                case raw:
                    return writer.write(starting_fut);
                    break;
                case structure:
                    return writer.write(struct_deser_fut);
                    break;
                case proto:
                    return writer.write(proto_converter_fut);
                    break;
                case proto_dser_fut:
                    return writer.write(proto_dser_fut);
                    break;
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
} // namespace srs

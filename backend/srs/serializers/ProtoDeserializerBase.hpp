#pragma once

#include <srs/data/message.pb.h>
#include <srs/serializers/StructDeserializer.hpp>

namespace srs
{
    template <typename Converter>
    class ProtoDeserializerBase
    {
      public:
        using InputType = const proto::Data*;
        using OutputType = std::string_view;
        using CoroType = asio::experimental::coro<OutputType(std::optional<InputType>)>;
        using InputFuture = boost::shared_future<std::optional<InputType>>;
        using OutputFuture = boost::shared_future<std::optional<OutputType>>;

        explicit ProtoDeserializerBase(asio::thread_pool& thread_pool, Converter converter)
            : converter_{ converter }
        {
            coro_ = generate_coro(thread_pool.get_executor());
            coro_sync_start(coro_, std::optional<InputType>{}, asio::use_awaitable);
        }

        auto create_future(InputFuture& pre_fut) -> OutputFuture { return create_coro_future(coro_, pre_fut); }
        [[nodiscard]] auto data() const -> const auto& { return output_data_; }

      private:
        std::string output_data_;
        CoroType coro_;
        Converter converter_;

        void reset() { output_data_.clear(); }

        // NOLINTNEXTLINE(readability-static-accessed-through-instance)
        auto generate_coro(asio::any_io_executor /*unused*/) -> CoroType
        {
            InputType temp_data{};
            while (true)
            {
                if (temp_data != nullptr)
                {
                    reset();
                    converter_(*temp_data, output_data_);
                }
                auto data = co_yield (std::string_view{ output_data_ });
                if (data.has_value())
                {
                    temp_data = data.value();
                }
                else
                {
                    spdlog::debug("Shutting down proto deserializer.");
                    co_return;
                }
            }
        }
    };

} // namespace srs

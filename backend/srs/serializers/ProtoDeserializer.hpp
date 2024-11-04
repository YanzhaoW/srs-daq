#pragma once

#include "message.pb.h"
#include <srs/serializers/StructDeserializer.hpp>

namespace srs
{
    class ProtoDeserializer
    {
      public:
        explicit ProtoDeserializer() = default;
        explicit ProtoDeserializer(asio::thread_pool& thread_pool)
        {
            coro_ = generate_coro(thread_pool.get_executor());
            coro_sync_start(coro_);
        }

        using InputType = const proto::Data*;
        using OutputType = std::string_view;
        using CoroType = asio::experimental::coro<OutputType(std::optional<InputType>)>;
        using InputFuture = boost::shared_future<std::optional<InputType>>;
        using OutputFuture = boost::shared_future<std::optional<OutputType>>;

        auto create_future(InputFuture& pre_fut) -> OutputFuture { return create_coro_future(coro_, pre_fut); }
        [[nodiscard]] auto data() const -> const auto& { return output_data_; }

      private:
        std::string output_data_;
        CoroType coro_;

        void reset() { output_data_.clear(); }

        auto convert(const proto::Data& proto_data) -> std::size_t
        {
            namespace protobuf = google::protobuf;
            namespace io = protobuf::io;
            auto output_stream = io::StringOutputStream{ &output_data_ };
            proto_data.SerializeToZeroCopyStream(&output_stream);
            return 0;
        }

        auto generate_coro(asio::any_io_executor /*unused*/) -> CoroType
        {
            InputType temp_data{};
            while (true)
            {
                if (temp_data == nullptr)
                {
                    reset();
                    convert(*temp_data);
                }
                auto data = co_yield (std::string_view{ output_data_ });
                if (data.has_value())
                {
                    temp_data = data.value();
                }
                else
                {
                    co_return;
                }
            }
        }
    };
} // namespace srs

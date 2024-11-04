#pragma once

#include <algorithm>
#include <boost/asio/experimental/coro.hpp>
#include <srs/Application.hpp>
#include <srs/analysis/DataStructs.hpp>
#include <srs/utils/CommonFunctions.hpp>

#include <zpp_bits.h>

namespace srs
{
    class StructDeserializer
    {
      public:
        using OutputType = const StructData*;
        using InputType = std::string_view;
        using CoroType = asio::experimental::coro<OutputType(std::optional<InputType>)>;
        using InputFuture = boost::shared_future<std::optional<InputType>>;
        using OutputFuture = boost::shared_future<std::optional<OutputType>>;

        explicit StructDeserializer(asio::thread_pool& thread_pool)
        {
            coro_ = generate_coro(thread_pool.get_executor());
            coro_sync_start(coro_, std::optional<InputType>{}, asio::use_awaitable);
        }

        auto create_future(InputFuture& pre_fut) -> OutputFuture { return create_coro_future(coro_, pre_fut); }

        [[nodiscard]] auto data() const -> const auto& { return output_data_; }

      private:
        ReceiveDataSquence receive_raw_data_;
        StructData output_data_;
        CoroType coro_;

        void reset() { reset_struct_data(output_data_); }

        auto generate_coro(asio::any_io_executor /*unused*/) -> CoroType
        {
            InputType temp_data{};
            while (true)
            {
                if (not temp_data.empty())
                {
                    reset();
                    convert(temp_data);
                }
                auto data = co_yield (&output_data_);
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

        // thread safe
        auto convert(std::string_view binary_data) -> std::size_t
        {
            auto translated_size = std::size_t{};
            auto deserialize_to = zpp::bits::in{ binary_data, zpp::bits::endian::network{}, zpp::bits::no_size{} };

            auto read_bytes = binary_data.size() * sizeof(BufferElementType);
            constexpr auto header_bytes = sizeof(output_data_.header);
            constexpr auto element_bytes = HIT_DATA_BIT_LENGTH / BYTE_BIT_LENGTH;
            auto vector_size = (read_bytes - header_bytes) / element_bytes;
            if (vector_size < 0)
            {
                throw std::runtime_error("Deserialization: Wrong header type!");
            }

            // locking mutex to prevent data racing
            receive_raw_data_.resize(vector_size);
            std::ranges::fill(receive_raw_data_, 0);
            deserialize_to(output_data_.header, receive_raw_data_).or_throw();
            byte_reverse_data_sq();
            translate_raw_data(output_data_);
            translated_size = receive_raw_data_.size();
            receive_raw_data_.clear();

            return translated_size;
        }

        void translate_raw_data(StructData& struct_data)
        {
            for (const auto& element : receive_raw_data_)
            {
                // spdlog::info("raw data: {:x}", element.to_ullong());
                if (auto is_hit = check_is_hit(element); is_hit)
                {
                    struct_data.hit_data.emplace_back(element);
                }
                else
                {
                    struct_data.marker_data.emplace_back(element);
                }
            }
        }

        static auto check_is_hit(const DataElementType& element) -> bool { return element.test(FLAG_BIT_POSITION); }

        void byte_reverse_data_sq()
        {
            // reverse byte order of each element.
            // TODO: This is current due to a bug from zpp_bits. The reversion is not needed if it's fixed.
            for (auto& element : receive_raw_data_)
            {
                element = byte_swap(element);
            }
        }
    };

} // namespace srs

#pragma once

#include <algorithm>
#include <mutex>
#include <srs/analysis/DataStructs.hpp>
#include <srs/utils/CommonFunctions.hpp>

#include <zpp_bits.h>

namespace srs
{
    class StructDeserializer
    {
      public:
        StructDeserializer() = default;

        using OutputType = StructData;
        using InputType = BinaryData;

        // thread safe
        auto convert(const InputType& binary_data, OutputType& struct_data) -> std::size_t
        {
            auto translated_size = std::size_t{};
            auto deserialize_to = zpp::bits::in{ binary_data, zpp::bits::endian::network{}, zpp::bits::no_size{} };

            auto read_bytes = binary_data.size() * sizeof(BufferElementType);
            constexpr auto header_bytes = sizeof(struct_data.header);
            constexpr auto element_bytes = HIT_DATA_BIT_LENGTH / BYTE_BIT_LENGTH;
            auto vector_size = (read_bytes - header_bytes) / element_bytes;
            if (vector_size < 0)
            {
                throw std::runtime_error("Deserialization: Wrong header type!");
            }

            // locking mutex to prevent data racing
            auto mutex_lock = std::scoped_lock{ mutex_ };
            receive_raw_data_.resize(vector_size);
            std::ranges::fill(receive_raw_data_, 0);
            deserialize_to(struct_data.header, receive_raw_data_).or_throw();
            byte_reverse_data_sq();
            translate_raw_data(struct_data);
            translated_size = receive_raw_data_.size();
            receive_raw_data_.clear();

            return translated_size;
        }

      private:
        ReceiveDataSquence receive_raw_data_;
        std::mutex mutex_;

        void translate_raw_data(OutputType& struct_data)
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

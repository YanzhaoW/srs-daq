#pragma once

#include <asio/thread_pool.hpp>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <vector>

namespace srs
{
    // General
    constexpr auto BYTE_BIT_LENGTH = 8;

    // Connections:
    constexpr auto WRITE_COMMAND_BITS = uint8_t{ 0xaa };
    constexpr auto DEFAULT_TYPE_BITS = uint8_t{ 0xaa };
    constexpr auto DEFAULT_CHANNEL_ADDRE = uint16_t{ 0xff };
    constexpr auto COMMAND_LENGTH_BITS = uint16_t{ 0xffff };
    constexpr auto ZERO_UINT16_PADDING = uint16_t{};
    constexpr auto SMALL_READ_MSG_BUFFER_SIZE = 100;
    constexpr auto LARGE_READ_MSG_BUFFER_SIZE = 10000;

    constexpr auto DEFAULT_CMD_LENGTH = uint16_t{ 0xffff };
    constexpr auto CMD_TYPE = uint8_t{ 0xaa };
    constexpr auto WRITE_CMD = uint8_t{ 0xaa };
    constexpr auto READ_CMD = uint8_t{ 0xbb };
    constexpr auto I2C_ADDRESS = uint16_t{ 0x0042 };  /* device address = 0x21 */
    constexpr auto NULL_ADDRESS = uint16_t{ 0x000f }; /* device address = 0x21 */

    constexpr auto INIT_COUNT_VALUE = uint32_t{ 0x80000000 };
    constexpr auto DEFAULT_STATUS_WAITING_TIME_SECONDS = std::chrono::seconds{ 4 };

    // port numbers:
    constexpr auto FEC_DAQ_RECEIVE_PORT = 6006;
    static constexpr int FEC_CONTROL_LOCAL_PORT = 6007;

    using BufferElementType = uint8_t;
    using WriteBufferType = std::vector<BufferElementType>;

    template <int buffer_size = SMALL_READ_MSG_BUFFER_SIZE>
    using ReadBufferType = std::array<BufferElementType, buffer_size>;

    using CommunicateEntryType = uint32_t;

    // Data processor:
    constexpr auto DEFAULT_DISPLAY_PERIOD = std::chrono::milliseconds{ 200 };
    constexpr auto FEC_ID_BIT_LENGTH = 8;
    constexpr auto HIT_DATA_BIT_LENGTH = 48;
    constexpr auto VMM_TAG_BIT_LENGTH = 3;
    constexpr auto SRS_TIMESTAMP_HIGH_BIT_LENGTH = 32;
    constexpr auto SRS_TIMESTAMP_LOW_BIT_LENGTH = 10;
    constexpr auto FLAG_BIT_POSITION = 15; // zero based

    enum class DataPrintMode
    {
        print_speed,
        print_header,
        print_raw,
        print_all
    };

    using io_context_type = asio::thread_pool;

    // subbits from a half open range [min, max)
    template <std::size_t bit_size, std::size_t max, std::size_t min = 0>
    inline constexpr auto subset(const std::bitset<bit_size>& bits) -> std::bitset<max - min>
    {
        constexpr auto max_size = 64;
        static_assert(max > min);
        static_assert(max_size >= (max - min));
        constexpr auto ignore_high = bit_size - max;

        auto new_bits = (bits << ignore_high) >> (ignore_high + min);
        return std::bitset<max - min>{ new_bits.to_ullong() };
    }

    template <std::size_t high_size, std::size_t low_size>
    inline constexpr auto merge_bits(const std::bitset<high_size>& high_bits,
                                     const std::bitset<low_size>& low_bits) -> std::bitset<high_size + low_size>
    {
        using NewBit = std::bitset<high_size + low_size>;
        constexpr auto max_size = 64;
        static_assert(max_size >= high_size + low_size);

        auto high_bits_part = NewBit(high_bits.to_ullong());
        auto low_bits_part = NewBit(low_bits.to_ullong());
        auto new_bits = (high_bits_part << low_size) | low_bits_part;
        return std::bitset<high_size + low_size>(new_bits.to_ullong());
    }

    template <std::size_t bit_size>
    inline constexpr auto byte_swap(const std::bitset<bit_size>& bits)
    {
        auto val = bits.to_ullong();
        val = val << (sizeof(uint64_t) * BYTE_BIT_LENGTH - bit_size);
        val = std::byteswap(val);
        return std::bitset<bit_size>(val);
    }

    template <typename T>
    constexpr auto gray_to_binary(T gray_val)
    {
        auto bin_val = T{ gray_val };
        while (gray_val > 0)
        {
            gray_val >>= 1;
            bin_val ^= gray_val;
        }
        return bin_val;
    }
} // namespace srs

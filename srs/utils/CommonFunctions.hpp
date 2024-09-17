#pragma once

#include "CommonDefitions.hpp"
#include <bit>
#include <bitset>

namespace srs
{
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

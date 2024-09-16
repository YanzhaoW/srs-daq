#pragma once

#include "CommonDefitions.hpp"
#include <array>
#include <bitset>
#include <cstdint>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <vector>

namespace srs
{
    struct ReceiveDataHeader
    {
        uint32_t frame_counter{};
        std::array<char, VMM_TAG_BIT_LENGTH> vmm_tag;
        uint8_t fec_id;
        uint32_t udp_timestamp{};
        uint32_t overflow{};
    };

    using DataElementType = std::bitset<HIT_DATA_BIT_LENGTH>;
    using ReceiveDataSquence = std::vector<DataElementType>;

    struct ReceiveData
    {
        ReceiveDataHeader header;
        ReceiveDataSquence data;
    };

    struct MarkerData
    {
        MarkerData() = default;
        explicit MarkerData(const std::bitset<HIT_DATA_BIT_LENGTH>& raw_data);
        uint8_t vmm_id{};
        uint64_t srs_timestamp{};
    };

    struct HitData
    {
        HitData() = default;
        explicit HitData(const std::bitset<HIT_DATA_BIT_LENGTH>& raw_data);
        bool is_over_threshold = false;
        uint8_t channel_num{};
        uint8_t tdc{};
        uint8_t offset{};
        uint8_t vmm_id{};
        uint16_t adc{};
        uint16_t bc_id{};
    };

} // namespace srs

template <>
class fmt::formatter<srs::ReceiveDataHeader>
{
  public:
    static constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }
    template <typename FmtContent>
    constexpr auto format(const srs::ReceiveDataHeader& header, FmtContent& ctn) const
    {
        return format_to(ctn.out(),
                         "frame counter: {}, vmm tag: {}, fec id: {:08b}, udp timestamp: {}, overflow: {}",
                         header.frame_counter,
                         fmt::join(header.vmm_tag, ""),
                         header.fec_id,
                         header.udp_timestamp,
                         header.overflow);
    }
};

template <>
class fmt::formatter<srs::MarkerData>
{
  public:
    static constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }
    template <typename FmtContent>
    constexpr auto format(const srs::MarkerData& marker, FmtContent& ctn) const
    {
        return format_to(ctn.out(), "vmm id: {}, srs timestamp: {}", marker.vmm_id, marker.srs_timestamp);
    }
};

template <>
class fmt::formatter<srs::HitData>
{
  public:
    static constexpr auto parse(format_parse_context& ctx) { return ctx.end(); }
    template <typename FmtContent>
    constexpr auto format(const srs::HitData& hit, FmtContent& ctn) const
    {
        return format_to(ctn.out(),
                         "Over threshold: {}, channel num: {}, tdc: {}, adc: {}, offset: {}, vmm id: {}, bc id: {}",
                         hit.is_over_threshold,
                         hit.channel_num,
                         hit.tdc,
                         hit.adc,
                         hit.offset,
                         hit.vmm_id,
                         hit.bc_id);
    }
};

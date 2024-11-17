#pragma once

#include <array>
#include <cstdint>
#include <vector>

#ifdef HAS_ROOT
#include <Rtypes.h>
#endif

namespace srs
{
    constexpr auto VMM_TAG_BIT_LENGTH = 3;
    struct ReceiveDataHeader
    {
        uint32_t frame_counter{};
        std::array<char, VMM_TAG_BIT_LENGTH> vmm_tag{};
        uint8_t fec_id{};
        uint32_t udp_timestamp{};
        uint32_t overflow{};
#ifdef HAS_ROOT
        ClassDefNV(ReceiveDataHeader, 1);
#endif
    };

    struct MarkerData
    {
        uint8_t vmm_id{};
        uint64_t srs_timestamp{};
#ifdef HAS_ROOT
        ClassDefNV(MarkerData, 1);
#endif
    };

    struct HitData
    {
        bool is_over_threshold = false;
        uint8_t channel_num{};
        uint8_t tdc{};
        uint8_t offset{};
        uint8_t vmm_id{};
        uint16_t adc{};
        uint16_t bc_id{};
#ifdef HAS_ROOT
        ClassDefNV(HitData, 1);
#endif
    };

    struct StructData
    {
        ReceiveDataHeader header{};
        std::vector<MarkerData> marker_data;
        std::vector<HitData> hit_data;
#ifdef HAS_ROOT
        ClassDefNV(StructData, 1);
#endif
    };

    inline void reset_struct_data(StructData& struct_data)
    {
        struct_data.header = ReceiveDataHeader{};
        struct_data.marker_data.clear();
        struct_data.hit_data.clear();
    }
} // namespace srs

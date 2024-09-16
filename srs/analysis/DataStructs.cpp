#include "DataStructs.hpp"
#include "CommonDefitions.hpp"
#include <bit>

namespace srs
{
    namespace
    {
        template <typename T>
        auto convert_to(const DataElementType& raw_data) -> T
        {
            constexpr auto struct_size = sizeof(uint64_t);
            static_assert(HIT_DATA_BIT_LENGTH <= BYTE_BIT_LENGTH * struct_size);
            static_assert(sizeof(T) == struct_size);
            auto expanded_raw_data = std::bitset<BYTE_BIT_LENGTH * struct_size>(raw_data.to_ullong());
            constexpr auto shifted_bits = struct_size * BYTE_BIT_LENGTH - HIT_DATA_BIT_LENGTH;
            expanded_raw_data = expanded_raw_data << shifted_bits;
            return std::bit_cast<T>(expanded_raw_data.to_ullong());
        }
    }; // namespace
    struct HitDataCompact
    {
        uint16_t : 16;
        uint16_t tdc : 8;
        uint16_t channel_num : 6;
        uint16_t is_over_threshold : 1;
        uint16_t flag : 1;
        uint32_t bc_id : 12;
        uint32_t adc : 10;
        uint32_t vmm_id : 5;
        uint32_t offset : 5;
    };

    struct MarkerDataCompact
    {
        uint16_t : 16;
        uint16_t timestamp_low_bits : SRS_TIMESTAMP_LOW_BIT_LENGTH;
        uint16_t vmm_id : 5;
        uint16_t flag : 1;
        uint32_t timestamp_high_bits : SRS_TIMESTAMP_HIGH_BIT_LENGTH;
    };

    MarkerData::MarkerData(const DataElementType& raw_data)
    {
        auto marker_data_compact = convert_to<MarkerDataCompact>(raw_data);

        auto timestamp_high_bits =
            std::bitset<SRS_TIMESTAMP_HIGH_BIT_LENGTH>(static_cast<uint32_t>(marker_data_compact.timestamp_high_bits));
        auto timestamp_low_bits =
            std::bitset<SRS_TIMESTAMP_LOW_BIT_LENGTH>(static_cast<uint32_t>(marker_data_compact.timestamp_low_bits));
        srs_timestamp =
            static_cast<decltype(srs_timestamp)>(merge_bits(timestamp_high_bits, timestamp_low_bits).to_ullong());
        vmm_id = static_cast<decltype(vmm_id)>(marker_data_compact.vmm_id);
    }

    HitData::HitData(const DataElementType& raw_data)
    {
        auto hit_data_compact = convert_to<HitDataCompact>(raw_data);

        is_over_threshold = static_cast<decltype(is_over_threshold)>(hit_data_compact.is_over_threshold);
        channel_num = static_cast<decltype(channel_num)>(hit_data_compact.channel_num);
        tdc = static_cast<decltype(tdc)>(hit_data_compact.tdc);
        offset = static_cast<decltype(offset)>(hit_data_compact.offset);
        vmm_id = static_cast<decltype(vmm_id)>(hit_data_compact.vmm_id);
        adc = static_cast<decltype(adc)>(hit_data_compact.adc);
        bc_id = static_cast<decltype(bc_id)>(hit_data_compact.bc_id);
        bc_id = gray_to_binary(bc_id);
    }

} // namespace srs

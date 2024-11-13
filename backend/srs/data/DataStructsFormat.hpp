#include <srs/data/DataStructs.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>

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

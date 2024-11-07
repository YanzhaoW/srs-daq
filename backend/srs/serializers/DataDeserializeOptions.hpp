#pragma once

#include <cstdint>

namespace srs
{
    enum class DataDeserializeOptions : uint8_t
    {
        none,
        raw,
        structure,
        proto,
        proto_frame
    };
}

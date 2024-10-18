#pragma once

#include <cstdint>

namespace srs
{
    enum class DataDeserializeOptions : uint8_t
    {
        binary,
        structure,
        proto
    };
}

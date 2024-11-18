#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

namespace srs
{
    enum class DataConvertOptions : uint8_t
    {
        none,
        raw,
        raw_frame,
        structure,
        proto,
        proto_frame
    };

    struct ConvertOptionRelation
    {
        constexpr ConvertOptionRelation(DataConvertOptions option, DataConvertOptions next_option)
            : dependee{ option }
            , depender{ next_option }
        {
        }
        DataConvertOptions dependee;
        DataConvertOptions depender;
    };

    constexpr auto EMPTY_CONVERT_OPTION_COUNT_MAP = []()
    {
        using enum DataConvertOptions;
        return std::array{ std::make_pair(raw, 0),
                           std::make_pair(raw_frame, 0),
                           std::make_pair(structure, 0),
                           std::make_pair(proto, 0),
                           std::make_pair(proto_frame, 0) };
    }();

    constexpr auto CONVERT_OPTION_RELATIONS = []()
    {
        using enum DataConvertOptions;
        return std::array{ ConvertOptionRelation{ raw, raw_frame },
                           ConvertOptionRelation{ raw, structure },
                           ConvertOptionRelation{ structure, proto },
                           ConvertOptionRelation{ structure, proto_frame } };
    }();

    constexpr auto convert_option_has_dependency(DataConvertOptions dependee, DataConvertOptions depender) -> bool
    {
        return std::ranges::any_of(CONVERT_OPTION_RELATIONS,
                                   [dependee, depender](ConvertOptionRelation relation) -> bool
                                   {
                                       if (dependee == relation.dependee)
                                       {
                                           if (depender == relation.depender)
                                           {
                                               return true;
                                           }
                                           return convert_option_has_dependency(relation.depender, depender);
                                       }

                                       // nothing is depending on it
                                       return false;
                                   });
    }

} // namespace srs
#pragma once

#include <cstdint>
#include <filesystem>
#include <srs/serializers/DataDeserializeOptions.hpp>
#include <string_view>
#include <utility>

namespace srs
{
    enum class DataWriterOption : uint8_t
    {

        no_output = 0x00,
        root = 0x01,
        json = 0x02,
        bin = 0x04,
        udp = 0x10,
    };
    // class DataWriterOption
    // {
    //   public:
    //     enum class Option : uint8_t
    //     {
    //         no_output = 0x00,
    //         root = 0x01,
    //         json = 0x02,
    //         bin = 0x04,
    //         udp = 0x10,
    //     };
    //     using enum Option;

    //     // NOLINTNEXTLINE (google-explicit-constructor)
    //     /*implicit*/ DataWriterOption(Option option)
    //         : option_{ option }
    //     {
    //     }

    //     void operator|=(DataWriterOption right)
    //     {
    //         auto res_underlying = std::to_underlying(option_);
    //         res_underlying |= std::to_underlying(right.option_);
    //         option_ = Option{ res_underlying };
    //     }

    //     auto operator==(DataWriterOption rhs) const -> bool { return ((*this) & rhs) != no_output; }
    //     auto operator==(Option rhs) const -> bool { return (*this) == DataWriterOption{ rhs }; }

    //   private:
    //     Option option_;
    //     auto operator&(DataWriterOption right) const -> Option
    //     {
    //         auto res_underlying = std::to_underlying(option_);
    //         res_underlying &= std::to_underlying(right.option_);
    //         return Option{ res_underlying };
    //     }
    // };

    inline auto get_filetype_from_filename(std::string_view filename)
        -> std::tuple<DataWriterOption, DataDeserializeOptions>
    {
        using enum DataWriterOption;
        using enum DataDeserializeOptions;
        namespace fs = std::filesystem;

        const auto file_ext = fs::path{ filename }.extension().string();
        if (file_ext == ".bin" or file_ext == ".lmd")
        {
            return { bin, raw };
        }
        if (file_ext == ".binpd")
        {
            return { bin, proto_frame };
        }
        if (file_ext == ".root")
        {
            return { root, structure };
        }
        if (file_ext == ".json")
        {
            return { json, structure };
        }
        if (auto pos = file_ext.find(':'); pos != std::string::npos)
        {
            return { udp, proto };
        }

        return { no_output, none };
    }

} // namespace srs

#pragma once
#include <cstdint>
#include <utility>

class DataWriterOption
{
  public:
    enum class Option : uint8_t
    {
        no_output = 0x0,
        root = 0x1,
        json = 0x2,
        binary = 0x4,
        udp = 0x8
    };
    using enum Option;

    // NOLINTNEXTLINE (google-explicit-constructor)
    /*implicit*/ DataWriterOption(Option option)
        : option_{ option }
    {
    }

    inline void operator|=(DataWriterOption right)
    {
        auto res_underlying = std::to_underlying(option_);
        res_underlying |= std::to_underlying(right.option_);
        option_ = Option{ res_underlying };
    }

    inline auto operator==(DataWriterOption rhs) const -> bool { return ((*this) & rhs) != no_output; }
    inline auto operator==(Option rhs) const -> bool { return (*this) == DataWriterOption{ rhs }; }

  private:
    Option option_;
    inline auto operator&(DataWriterOption right) const -> Option
    {
        auto res_underlying = std::to_underlying(option_);
        res_underlying &= std::to_underlying(right.option_);
        return Option{ res_underlying };
    }
};

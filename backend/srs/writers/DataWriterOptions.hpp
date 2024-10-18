#pragma once
#include <cstdint>
#include <utility>

class DataWriterOption
{
  public:
    enum class Option : uint8_t
    {
        no_output = 0x00,
        root = 0x01,
        json = 0x02,
        bin = 0x04,
        bin_pd = 0x08,
        udp_bin = 0x10,
        udp_pd = 0x20
    };
    using enum Option;

    // NOLINTNEXTLINE (google-explicit-constructor)
    /*implicit*/ DataWriterOption(Option option)
        : option_{ option }
    {
    }

    void operator|=(DataWriterOption right)
    {
        auto res_underlying = std::to_underlying(option_);
        res_underlying |= std::to_underlying(right.option_);
        option_ = Option{ res_underlying };
    }

    auto operator==(DataWriterOption rhs) const -> bool { return ((*this) & rhs) != no_output; }
    auto operator==(Option rhs) const -> bool { return (*this) == DataWriterOption{ rhs }; }

  private:
    Option option_;
    auto operator&(DataWriterOption right) const -> Option
    {
        auto res_underlying = std::to_underlying(option_);
        res_underlying &= std::to_underlying(right.option_);
        return Option{ res_underlying };
    }
};

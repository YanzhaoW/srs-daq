#pragma once

#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <srs/serializers/Concepts.hpp>
#include <string>

namespace srs
{
    template <StreamableOutput Deserializer>
    class BinaryWriter
    {
      public:
        using OutputType = Deserializer::OutputType;
        explicit BinaryWriter(const std::string& filename, Deserializer& deserializer)
            : ofstream_{ filename, std::ios::trunc }
            , deserializer_{ deserializer }
        {
            if (not ofstream_.is_open())
            {
                throw std::runtime_error(fmt::format("Filename {:?} cannot be open!", filename));
            }
        }

        void write_to_file(const OutputType& data) { ofstream_ << data; }

        void close() { ofstream_.close(); }

      private:
        std::ofstream ofstream_;
        std::reference_wrapper<Deserializer> deserializer_;
    };
} // namespace srs

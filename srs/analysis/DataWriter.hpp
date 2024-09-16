#pragma once

#include "DataStructs.hpp"

namespace srs
{
    enum class DataWriterOption
    {
        root,
        json,
        binary,
        udp
    };
    class DataWriter
    {
      public:
        using enum DataWriterOption;
        DataWriter() = default;
        void write_binary(std::span<BufferElementType> read_data);
        void write_struct(const ReceiveData& read_data);

        // setters:
        void set_write_option(DataWriterOption option);
        void set_output_file_basename(std::string_view filename);

        // Getter:
        [[nodiscard]] auto get_write_option() const -> DataWriterOption { return write_option_; }

      private:
        DataWriterOption write_option_ = binary;
        std::string output_basename_ = "output";

        void write_binary_file(std::span<BufferElementType> read_data);
        void write_binary_udp(std::span<BufferElementType> read_data);

    };

} // namespace srs

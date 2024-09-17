#pragma once

#include "DataStructs.hpp"
#include <fstream>

namespace srs
{
    class RootFileSink;
    class DataProcessor;

    class DataWriter
    {
      public:
        using enum DataWriterOption;
        explicit DataWriter(DataProcessor* processor);

        ~DataWriter();
        DataWriter(const DataWriter&) = delete;
        DataWriter& operator=(const DataWriter&) = delete;
        DataWriter(DataWriter&&) = default;
        DataWriter& operator=(DataWriter&&) = default;

        void write_binary(const WriteBufferType& read_data);
        void write_struct(ExportData& read_data);

        // setters:
        void set_write_option(DataWriterOption option);
        void set_output_file_basename(std::string_view filename);

        // Getter:
        [[nodiscard]] auto get_write_option() const -> DataWriterOption { return write_option_; }
        [[nodiscard]] auto is_binary() const -> bool { return write_option_ == binary or write_option_ == udp; }
        [[nodiscard]] auto is_struct() const -> bool { return write_option_ == root or write_option_ == json; }

      private:
        DataWriterOption write_option_ = binary;
        std::string output_basename_ = "output";
        std::unique_ptr<std::ofstream> binary_file_;
        std::unique_ptr<std::ofstream> json_file_;
        std::unique_ptr<RootFileSink> root_file_;
        DataProcessor* data_processor_ = nullptr;

        void write_struct_json(const ExportData& data_struct);
        void write_struct_root(ExportData& data_struct);

        void write_binary_file(const WriteBufferType& read_data);
        static void write_binary_udp(const WriteBufferType& read_data);
    };

} // namespace srs

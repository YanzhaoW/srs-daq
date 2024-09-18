#pragma once

#include "DataStructs.hpp"
#include "DataWriterOptions.hpp"
#include <fstream>
#include "JsonWriter.hpp"
#include <map>

namespace srs
{
    class RootFileSink;
    class DataProcessor;

    class DataWriter
    {
      public:
        using enum DataWriterOption::Option;
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
        void set_output_filenames(std::vector<std::string> filenames);

        // Getter:
        [[nodiscard]] auto get_write_option() const -> DataWriterOption { return write_option_; }
        [[nodiscard]] auto is_binary() const -> bool { return write_option_ == binary or write_option_ == udp; }
        [[nodiscard]] auto is_struct() const -> bool { return write_option_ == root or write_option_ == json; }

      private:
        DataWriterOption write_option_ = no_output;
        std::map<DataWriterOption::Option, std::vector<std::string>> output_filenames{ { binary, {} },
                                                                                       { root, {} },
                                                                                       { json, {} },
                                                                                       { udp, {} } };
        std::unique_ptr<std::ofstream> binary_file_;
        std::unique_ptr<JsonWriter> json_file_;
        std::unique_ptr<RootFileSink> root_file_;
        DataProcessor* data_processor_ = nullptr;

        void write_struct_json(const ExportData& data_struct);
        void write_struct_root(ExportData& data_struct);

        void write_binary_file(const WriteBufferType& read_data);
        static void write_binary_udp(const WriteBufferType& read_data);
    };

} // namespace srs

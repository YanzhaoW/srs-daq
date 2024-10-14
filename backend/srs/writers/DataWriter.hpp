#pragma once

#include "DataStructs.hpp"
#include "DataWriterOptions.hpp"
#include "JsonWriter.hpp"
#include <fstream>
#include <map>

namespace srs
{
    class RootFileWriter;
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

        void write_binary(const BinaryData& read_data);
        void write_struct(StructData& read_data);

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
#ifdef HAS_ROOT
        std::unique_ptr<RootFileWriter> root_file_;
#endif
        DataProcessor* data_processor_ = nullptr;

        void write_struct_json(const StructData& data_struct);
        void write_struct_root(StructData& data_struct);

        void write_binary_file(const BinaryData& read_data);
        static void write_binary_udp(const BinaryData& read_data);
    };

} // namespace srs

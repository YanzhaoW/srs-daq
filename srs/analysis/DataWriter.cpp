#include "DataWriter.hpp"
#include <TFile.h>
#include <map>
#include <spdlog/spdlog.h>

namespace srs
{
    using enum DataWriterOption;
    const auto file_suffix =
        std::map<DataWriterOption, std::string>{ { root, "root" }, { json, "json" }, { binary, "lmd" } };

    void DataWriter::set_write_option(DataWriterOption option)
    {
        if (option == root)
        {
#ifndef HAS_ROOT
            throw std::runtime_error(
                "DataWriter: cannot output to a root file. Please make sure srs-daq is built with ROOT library.");
#else
            spdlog::debug("DataWriter: Write to root file is viable.");
#endif
        }
        write_option_ = option;
    }

    DataWriter::DataWriter() = default;
    DataWriter::~DataWriter() = default;
    void DataWriter::write_binary(const WriteBufferType& read_data)
    {
        if (write_option_ == binary)
        {
            write_binary_file(read_data);
        }
        else if (write_option_ == udp)
        {
            write_binary_udp(read_data);
        }
        else
        {
            throw std::runtime_error(
                R"(DataWriter: write_binary is called but write option is not set to either "binary" or "udp")");
        }
    }

    void DataWriter::write_struct(const ReceiveData& read_data)
    {
        if (write_option_ == json)
        {
            write_struct_json(read_data);
        }
        else if (write_option_ == root)
        {
            write_struct_root(read_data);
        }
        else
        {
            throw std::runtime_error(
                R"(DataWriter: write_struct is called but write option is not set to either "root" or "json")");
        }
    }

    void DataWriter::write_struct_json(const ReceiveData& read_data) {}
    void DataWriter::write_struct_root(const ReceiveData& read_data) {}

    void DataWriter::write_binary_file(const WriteBufferType& read_data)
    {
        if (binary_file_ == nullptr)
        {
            auto filename = fmt::format("{}.{}", output_basename_, file_suffix.at(write_option_));
            spdlog::info("DataWriter: Writing raw data to a binary file {:?}", filename);
            binary_file_ =
                std::make_unique<std::ofstream>(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        }
        binary_file_->write(reinterpret_cast<const char*>(read_data.data()),
                            static_cast<int64_t>(read_data.size() * sizeof(BufferElementType) / sizeof(char)));
    }

    void DataWriter::write_binary_udp(const WriteBufferType& /*read_data*/)
    {
        throw std::logic_error("DataWriter: udp file output is not yet implemented. Please use other write options.");
    }

} // namespace srs

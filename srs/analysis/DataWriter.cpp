#include "DataWriter.hpp"
#include <map>
#include <spdlog/spdlog.h>

namespace srs
{
    using enum DataWriterOption;
    const auto file_suffix =
        std::map<DataWriterOption, std::string>{ { root, ".root" }, { json, ".json" }, { binary, ".lmd" } };

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

    void DataWriter::write_binary(std::span<BufferElementType> read_data)
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

    void DataWriter::write_binary_file(std::span<BufferElementType> read_data) {}

    void DataWriter::write_binary_udp(std::span<BufferElementType> read_data) {}

    void DataWriter::write_struct(const ReceiveData& read_data) {}

} // namespace srs

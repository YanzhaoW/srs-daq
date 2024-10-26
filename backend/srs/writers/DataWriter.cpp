#include "DataWriter.hpp"
#include "DataProcessor.hpp"
#include "RootFileWriter.hpp"
#include <asio/ip/address.hpp>
#include <filesystem>
#include <map>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace srs
{
    using enum DataWriterOption::Option;

    namespace
    {

        auto check_is_ip_address(const auto& ip_port) -> bool
        {
            const auto colon_pos = ip_port.find(":");
            if (colon_pos == std::string::npos)
            {
                spdlog::critical("Ill format socket string {:?}. Please set it as \"ip:port\"", ip_port);
                return false;
            }
            const auto ip_string = ip_port.substr(0, colon_pos);
            auto err_code = std::error_code{};
            const auto addr = asio::ip::make_address(ip_string, err_code);
            if (err_code)
            {
                spdlog::critical("Cannot parse ip address {:?}. Error code: {}", ip_string, err_code.message());
                return false;
            }
            return true;
        }

    } // namespace

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

    DataWriter::DataWriter(DataProcessor* processor)
        : data_processor_{ processor }
    {
    }

    DataWriter::~DataWriter() = default;
    void DataWriter::write_binary(const BinaryData& read_data)
    {
        if (write_option_ == bin)
        {
            write_binary_file(read_data);
        }

        if (write_option_ == udp_bin)
        {
            write_binary_udp(read_data);
        }

        if (not(write_option_ == bin) and not(write_option_ == udp_bin))
        {
            throw std::runtime_error(
                R"(DataWriter: write_binary is called but write option is not set to either "binary" or "udp")");
        }
    }

    void DataWriter::write_struct(const StructData& read_data)
    {
        if (write_option_ == json)
        {
            write_struct_json(read_data);
        }

        if (write_option_ == root)
        {
            write_struct_root(read_data);
        }

        if (not(write_option_ == json) and not(write_option_ == root))
        {
            throw std::runtime_error(
                R"(DataWriter: write_struct is called but write option is not set to either "root" or "json")");
        }
    }

    void DataWriter::write_struct_json(const StructData& data_struct)
    {
        if (json_file_ == nullptr)
        {
            const auto& json_files = output_filenames.at(json);
            if (json_files.size() > 1)
            {
                spdlog::warn("DataWriter: Multiple json output files detected: {:?}. Only the first one is used",
                             fmt::join(json_files, ", "));
            }
            spdlog::info("DataWriter: Writing data structure to a json file {:?}", json_files.front());
            json_file_ = std::make_unique<JsonWriter>(json_files.front());
            json_file_->write(data_struct);
        }
        json_file_->write(",");
        json_file_->write(data_struct);
    }

    void DataWriter::write_struct_root(const StructData& data_struct)
    {
#ifdef HAS_ROOT
        if (root_file_ == nullptr)
        {
            const auto& root_files = output_filenames.at(root);
            if (root_files.size() > 1)
            {
                spdlog::warn("DataWriter: Multiple root output files detected: {:?}. Only the first one is used",
                             fmt::join(root_files, ", "));
            }
            spdlog::info("DataWriter: Writing data structure to a root file {:?}", root_files.front());
            root_file_ = std::make_unique<RootFileWriter>(root_files.front().c_str(), "recreate");
            root_file_->register_branch(data_struct);
        }

        root_file_->fill();
#endif
    }

    void DataWriter::write_binary_file(const BinaryData& read_data)
    {
        if (binary_file_ == nullptr)
        {
            const auto& bin_files = output_filenames.at(bin);
            if (bin_files.size() > 1)
            {
                spdlog::warn("DataWriter: Multiple binary output files detected: {:?}. Only the first one is used",
                             fmt::join(bin_files, ", "));
            }
            spdlog::info("DataWriter: Writing raw data to a binary file {:?}", bin_files.front());
            binary_file_ =
                std::make_unique<std::ofstream>(bin_files.front(), std::ios::out | std::ios::binary | std::ios::trunc);
        }
        binary_file_->write(reinterpret_cast<const char*>(read_data.data()),
                            static_cast<int64_t>(read_data.size() * sizeof(BufferElementType) / sizeof(char)));
    }

    auto DataWriter::has_deserialize_type(DataDeserializeOptions option) const -> bool
    {
        switch (option)
        {
            case binary:
                return write_option_ == bin or write_option_ == udp_bin;
            case structure:
                return write_option_ == root or write_option_ == json;
            case proto:
                return write_option_ == bin_pd or write_option_ == udp_pd;
            default:
                return false;
        }
    }

    void DataWriter::set_output_filenames(std::vector<std::string> filenames)
    {
        for (auto& filename : filenames)
        {
            const auto file_ext = fs::path{ filename }.extension();
            if (file_ext == ".bin" or file_ext == ".lmd")
            {
                output_filenames[bin].push_back(std::move(filename));
                write_option_ |= bin;
            }
            else if (file_ext == ".root")
            {
#ifdef HAS_ROOT
                output_filenames[root].push_back(std::move(filename));
                write_option_ |= root;
#else
                spdlog::critical("This program is not compiled with the cern ROOT library. Output root file {:?} will "
                                 "not be generated!",
                                 filename);
#endif
            }
            else if (file_ext == ".json")
            {
                output_filenames[json].push_back(std::move(filename));
                write_option_ |= json;
            }
            else if (check_is_ip_address(filename))
            {
                output_filenames[udp_bin].push_back(std::move(filename));
                write_option_ |= udp_bin;
            }
            else
            {
                spdlog::critical(
                    "Unrecognized file extension {:?}. Available file extensions are .bin, .lmd, .root, .json",
                    file_ext.c_str());
                throw std::runtime_error("Error occured at DataWriter");
            }
        }
    }

    void DataWriter::write_binary_udp(const BinaryData& /*read_data*/)
    {
        throw std::logic_error("DataWriter: udp file output is not yet implemented. Please use other write options.");
    }

} // namespace srs

#include "DataProcessor.hpp"
#include "DataWriter.hpp"
#include "JsonWriter.hpp"
#include "RootFileWriter.hpp"
#include <boost/asio/ip/address.hpp>
#include <spdlog/spdlog.h>
#include <srs/writers/BinaryFileWriter.hpp>
#include <srs/writers/UDPWriter.hpp>

namespace srs
{
    namespace
    {
        auto convert_str_to_endpoint(asio::thread_pool& thread_pool, std::string_view ip_port)
            -> std::optional<asio::ip::udp::endpoint>
        {
            const auto colon_pos = ip_port.find(':');
            if (colon_pos == std::string::npos)
            {
                spdlog::critical("Ill format socket string {:?}. Please set it as \"ip:port\"", ip_port);
                return {};
            }
            auto ip_string = ip_port.substr(0, colon_pos);
            auto port_str = ip_port.substr(colon_pos + 1);

            auto err_code = boost::system::error_code{};
            auto resolver = asio::ip::udp::resolver{ thread_pool };
            auto query = asio::ip::udp::resolver::query{ std::string{ ip_string }, std::string{ port_str } };
            auto iter = resolver.resolve(query, err_code);
            if (err_code)
            {
                spdlog::critical("Cannot query the ip address {:?}. Error code: {}", ip_port, err_code.message());
                return {};
            }
            return *iter;
        }

    } // namespace

    DataWriter::DataWriter(DataProcessor* processor)
        : data_processor_{ processor }
    {
    }

    DataWriter::~DataWriter() = default;

    //     void DataWriter::write_struct_root(const StructData& data_struct)
    //     {
    // #ifdef HAS_ROOT
    //         if (root_file_ == nullptr)
    //         {
    //             const auto& root_files = output_filenames.at(root);
    //             if (root_files.size() > 1)
    //             {
    //                 spdlog::warn("DataWriter: Multiple root output files detected: {:?}. Only the first one is used",
    //                              fmt::join(root_files, ", "));
    //             }
    //             spdlog::info("DataWriter: Writing data structure to a root file {:?}", root_files.front());
    //             root_file_ = std::make_unique<RootFileWriter>(root_files.front().c_str(), "recreate");
    //             root_file_->register_branch(data_struct);
    //         }

    //         root_file_->fill();
    // #endif
    //     }

    auto DataWriter::has_deserialize_type(DataDeserializeOptions option) const -> bool
    {
        switch (option)
        {
            case raw:
                return check_if_exist(raw) or check_if_exist(structure) or check_if_exist(proto) or
                       check_if_exist(proto_frame);
            case structure:
                return check_if_exist(structure) or check_if_exist(proto) or check_if_exist(proto_frame);
            case proto:
                return check_if_exist(proto);
            case proto_frame:
                return check_if_exist(proto_frame);
            default:
                return false;
        }
    }

    void DataWriter::wait_for_finished() { boost::wait_for_all(write_futures_.begin(), write_futures_.end()); }

    void DataWriter::set_output_filenames(const std::vector<std::string>& filenames)
    {
        auto& app = data_processor_->get_app();
        // auto executor = app.get_io_context().get_executor();

        for (const auto& filename : filenames)
        {
            const auto [filetype, deser_mode] = get_filetype_from_filename(filename);

            auto is_ok = false;

            switch (filetype)
            {
                case no_output:
                    spdlog::error("Extension of the filename {:?} cannot be recognized!", filename);
                    break;
                case bin:
                {
                    is_ok =
                        binary_files_
                            .try_emplace(filename,
                                         std::make_unique<BinaryFileWriter>(app.get_io_context(), filename, deser_mode))
                            .second;
                    break;
                }
                case udp:
                {
                    auto endpoint = convert_str_to_endpoint(app.get_io_context(), filename);
                    if (endpoint.has_value())
                    {
                        is_ok =
                            udp_files_
                                .try_emplace(filename,
                                             std::make_unique<UDPWriter>(app, std::move(endpoint.value()), deser_mode))
                                .second;
                    }
                    break;
                }
                case root:
                {
#ifndef HAS_ROOT
                    throw std::runtime_error("DataWriter: cannot output to a root file. Please make sure srs-daq is "
                                             "built with ROOT library.");
#else
                    spdlog::debug("DataWriter: Write to root file is viable.");
#endif
                    // TODO: to be implemented
                    // increament_output_tracker(is_ok, filename, filetype);
                    break;
                }
                case json:
                {
                    // TODO: to be implemented
                }
            }

            if (is_ok)
            {
                spdlog::info("Add the output source {:?}", filename);
                ++(output_num_trackers_.at(deser_mode));
            }
            else
            {
                spdlog::error("The filename {:?} has been already added!", filename);
            }
        }
    }
} // namespace srs

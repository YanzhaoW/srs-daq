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

        auto check_root_dependency() -> bool
        {
#ifdef HAS_ROOT
            return true;
#else
            spdlog::error("Cannot output to a root file. Please make sure the program is "
                          "built with the ROOT library.");
            return false;
#endif
        }

    } // namespace

    DataWriter::DataWriter(DataProcessor* processor)
        : data_processor_{ processor }
    {
    }

    DataWriter::~DataWriter() = default;

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

    auto DataWriter::add_binary_file(const std::string& filename, DataDeserializeOptions deser_mode) -> bool
    {
        auto& app = data_processor_->get_app();
        return binary_files_
            .try_emplace(filename, std::make_unique<BinaryFileWriter>(app.get_io_context(), filename, deser_mode))
            .second;
    }

    auto DataWriter::add_udp_file(const std::string& filename, DataDeserializeOptions deser_mode) -> bool
    {
        auto& app = data_processor_->get_app();
        auto endpoint = convert_str_to_endpoint(app.get_io_context(), filename);
        if (endpoint.has_value())
        {
            return udp_files_
                .try_emplace(filename, std::make_unique<UDPWriter>(app, std::move(endpoint.value()), deser_mode))
                .second;
        }
        return false;
    }

    auto DataWriter::add_root_file(const std::string& filename) -> bool
    {
#ifdef HAS_ROOT
        auto& app = data_processor_->get_app();
        return root_files_
            .try_emplace(filename, std::make_unique<RootFileWriter>(app.get_io_context(), filename.c_str(), "RECREATE"))
            .second;
#else
        return false;
#endif
    }

    auto DataWriter::add_json_file(const std::string& filename) -> bool
    {
        auto& app = data_processor_->get_app();
        return json_files_.try_emplace(filename, std::make_unique<JsonWriter>(app.get_io_context(), filename)).second;
    }

    void DataWriter::set_output_filenames(const std::vector<std::string>& filenames)
    {
        auto& app = data_processor_->get_app();

        for (const auto& filename : filenames)
        {
            const auto [filetype, deser_mode] = get_filetype_from_filename(filename);

            auto is_ok = false;

            switch (filetype)
            {
                case no_output:
                    spdlog::error("Extension of the filename {:?} cannot be recognized!", filename);
                    continue;
                    break;
                case bin:
                    is_ok = add_binary_file(filename, deser_mode);
                    break;
                case udp:
                    is_ok = add_udp_file(filename, deser_mode);
                    break;
                case root:
                    if (not check_root_dependency())
                    {
                        continue;
                    }
                    is_ok = add_root_file(filename);
                    break;
                case json:
                    is_ok = add_json_file(filename);
                    break;
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

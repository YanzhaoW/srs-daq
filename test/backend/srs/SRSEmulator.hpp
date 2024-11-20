#pragma once

#include <boost/asio.hpp>
#include <boost/thread/future.hpp>
#include <optional>
#include <print>
#include <srs/readers/RawFrameReader.hpp>
#include <srs/utils/CommonAlias.hpp>
#include <srs/writers/UDPWriter.hpp>
#include <string>

namespace srs::test
{
    class SRSEmulator
    {
      public:
        explicit SRSEmulator(std::string_view filename, int port, App& app)
            : source_filename_{ filename }
            , frame_reader_{ source_filename_ }
            , udp_writer_{ app, asio::ip::udp::endpoint{ udp::v4(), static_cast<asio::ip::port_type>(port) } }
        {
        }

        void start_send_data()
        {
            while (true)
            {
                auto read_str = frame_reader_.read_one_frame();
                if (read_str.empty())
                {
                    return;
                }
                auto send_fut = boost::async([read_str]() { return std::optional<std::string_view>{ read_str }; });
                udp_writer_.write(std::move(send_fut)).get();
            }
        }

        // Getters
        auto get_remote_endpoint() -> const auto& { return udp_writer_.get_remote_endpoint(); }

      private:
        std::string source_filename_;
        RawFrameReader frame_reader_;
        UDPWriter udp_writer_;
    };
} // namespace srs::test

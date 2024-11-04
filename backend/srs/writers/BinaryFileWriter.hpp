#pragma once

#include <DataWriterOptions.hpp>
#include <boost/asio.hpp>
#include <boost/asio/experimental/coro.hpp>
#include <fmt/format.h>
#include <fstream>
#include <srs/analysis/DataProcessManager.hpp>
#include <srs/utils/CommonConcepts.hpp>
#include <string>

namespace srs
{
    class BinaryFileWriter
    {
      public:
        explicit BinaryFileWriter(asio::thread_pool& thread_pool,
                                  const std::string& filename,
                                  DataDeserializeOptions deser_mode)
            : deser_mode_{ deser_mode }
            , ofstream_{ filename, std::ios::trunc }
        {
            if (not ofstream_.is_open())
            {
                throw std::runtime_error(fmt::format("Filename {:?} cannot be open!", filename));
            }
            coro_ = write_string_to_file(thread_pool.get_executor());
            coro_sync_start(coro_, std::optional<std::string_view>{}, asio::use_awaitable);
        }

        static constexpr auto IsStructType = false;

        auto write(auto pre_future) -> boost::unique_future<std::optional<int>>
        {
            return create_coro_future(coro_, pre_future);
        }

        auto is_deserialize_valid() { return deser_mode_ == raw or deser_mode_ == proto_frame; }

        auto get_deserialize_mode() const -> DataDeserializeOptions { return deser_mode_; }

        void close() { ofstream_.close(); }

        auto get_filetype() const -> DataDeserializeOptions { return deser_mode_; }

      private:
        using enum DataDeserializeOptions;
        DataDeserializeOptions deser_mode_ = DataDeserializeOptions::none;
        std::ofstream ofstream_;
        // std::string_view continuous_send_msg_;
        asio::experimental::coro<int(std::optional<std::string_view>)> coro_;

        auto write_string_to_file(asio::any_io_executor /*unused*/)
            -> asio::experimental::coro<int(std::optional<std::string_view>)>
        {
            auto write_msg = std::string_view{};
            while (true)
            {
                if (not write_msg.empty())
                {
                    ofstream_ << write_msg;
                }
                auto msg = co_yield static_cast<int>(write_msg.size());

                if (msg.has_value())
                {
                    write_msg = msg.value();
                }
                else
                {
                    close();
                    co_return;
                }
            }
        }
    };
} // namespace srs

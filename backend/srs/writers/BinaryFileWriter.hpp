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
        using InputType = std::string_view;
        using OutputType = int;
        using CoroType = asio::experimental::coro<OutputType(std::optional<InputType>)>;
        using InputFuture = boost::shared_future<std::optional<InputType>>;
        using OutputFuture = boost::unique_future<std::optional<OutputType>>;
        static constexpr auto IsStructType = false;

        explicit BinaryFileWriter(asio::thread_pool& thread_pool,
                                  const std::string& filename,
                                  DataConvertOptions deser_mode)
            : convert_mode_{ deser_mode }
            , file_name_{ filename }
            , ofstream_{ filename, std::ios::trunc }
        {
            if (not ofstream_.is_open())
            {
                throw std::runtime_error(fmt::format("Filename {:?} cannot be open!", filename));
            }
            coro_ = generate_coro(thread_pool.get_executor());
            coro_sync_start(coro_, std::optional<InputType>{}, asio::use_awaitable);
        }
        auto write(auto pre_future) -> OutputFuture { return create_coro_future(coro_, pre_future); }
        auto get_convert_mode() const -> DataConvertOptions { return convert_mode_; }
        void close() { ofstream_.close(); }

      private:
        DataConvertOptions convert_mode_ = DataConvertOptions::none;
        std::string file_name_;
        std::ofstream ofstream_;
        CoroType coro_;

        // NOLINTNEXTLINE(readability-static-accessed-through-instance)
        auto generate_coro(asio::any_io_executor /*unused*/) -> CoroType
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
                    spdlog::info("Binary file {} is closed successfully", file_name_);
                    co_return;
                }
            }
        }
    };
} // namespace srs

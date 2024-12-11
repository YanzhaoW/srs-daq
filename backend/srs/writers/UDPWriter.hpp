#pragma once

#include <boost/asio.hpp>
#include <srs/analysis/DataProcessManager.hpp>
#include <srs/converters/DataConvertOptions.hpp>
#include <srs/utils/ConnectionBase.hpp>
#include <srs/writers/DataWriterOptions.hpp>

namespace srs
{
    class UDPWriterConnection : public ConnectionBase<>
    {
      public:
        explicit UDPWriterConnection(const ConnectionInfo& info)
            : ConnectionBase(info, "UDP writer")
        {
        }
    };

    class UDPWriter
    {
      public:
        UDPWriter(App& app, asio::ip::udp::endpoint endpoint, DataConvertOptions derser_mode = DataConvertOptions::none)
            : convert_mode_{ derser_mode }
            , connection_{ ConnectionInfo{ &app } }
            , app_{ app }
        {
            connection_.set_socket(std::make_unique<asio::ip::udp::socket>(
                app.get_io_context(), asio::ip::udp::endpoint{ asio::ip::udp::v4(), 0 }));
            connection_.set_remote_endpoint(std::move(endpoint));
            coro_ = connection_.send_continuous_message();
            common::coro_sync_start(coro_, std::optional<std::string_view>{}, asio::use_awaitable);
        }

        static constexpr auto IsStructType = false;
        auto is_deserialize_valid() { return convert_mode_ == raw or convert_mode_ == proto; }

        auto get_convert_mode() const -> DataConvertOptions { return convert_mode_; }
        auto write(auto last_fut) -> boost::unique_future<std::optional<int>>
        {
            return common::create_coro_future(coro_, last_fut);
        }

        // INFO: this will be called in coroutine
        void close() { connection_.close(); }

        // Getters:
        auto get_local_socket() -> const auto& { return connection_.get_socket(); }
        auto get_remote_endpoint() -> const auto& { return connection_.get_remote_endpoint(); }

      private:
        using enum DataConvertOptions;
        DataConvertOptions convert_mode_;
        UDPWriterConnection connection_;
        std::reference_wrapper<App> app_;
        asio::experimental::coro<int(std::optional<std::string_view>)> coro_;
    };

} // namespace srs

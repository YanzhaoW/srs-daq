#pragma once

#include <boost/asio.hpp>
#include <srs/analysis/DataProcessManager.hpp>
#include <srs/serializers/DataDeserializeOptions.hpp>
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
        UDPWriter(App& app, std::string_view ip_addr, asio::ip::port_type port_num, DataDeserializeOptions derser_mode)
            : deser_mode_{ derser_mode }
            , connection_{ ConnectionInfo{ &app } }
            , app_{ app }
        {
            connection_.set_socket(std::make_unique<asio::ip::udp::socket>(
                app.get_io_context(), asio::ip::udp::endpoint{ asio::ip::udp::v4(), 0 }));
            connection_.set_remote_endpoint(asio::ip::udp::endpoint{ asio::ip::make_address(ip_addr), port_num });
            coro_ = connection_.send_continuous_message();
            coro_sync_start(coro_, std::optional<std::string_view>{}, asio::use_awaitable);
        }

        static constexpr auto IsStructType = false;
        auto is_deserialize_valid() { return deser_mode_ == raw or deser_mode_ == proto; }

        auto get_deserialize_mode() const -> DataDeserializeOptions { return deser_mode_; }
        auto write(auto last_fut) -> boost::unique_future<std::optional<int>>
        {
            return create_coro_future(coro_, last_fut);
        }

        // INFO: this will be called in coroutine
        void close() { connection_.close_socket(); }

      private:
        using enum DataDeserializeOptions;
        DataDeserializeOptions deser_mode_ = DataDeserializeOptions::none;
        UDPWriterConnection connection_;
        std::reference_wrapper<App> app_;
        asio::experimental::coro<int(std::optional<std::string_view>)> coro_;
    };

} // namespace srs

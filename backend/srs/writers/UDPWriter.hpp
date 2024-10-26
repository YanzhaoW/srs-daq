#pragma once

#include <asio.hpp>
#include <srs/serializers/DataDeserializeOptions.hpp>
#include <srs/serializers/Deserializers.hpp>
#include <srs/utils/ConnectionBase.hpp>

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

    template <DataDeserializeOptions option>
    class UDPWriter
    {
      public:
        UDPWriter(App& app, std::string_view ip_addr, asio::ip::port_type port_num)
            : connection_{ ConnectionInfo{ &app } }
            , app_{ app }
        {
            connection_.set_socket(std::make_unique<asio::ip::udp::socket>(
                app.get_io_context(), asio::ip::udp::endpoint{ asio::ip::udp::v4(), 0 }));
            connection_.set_remote_endpoint(asio::ip::udp::endpoint{ asio::ip::make_address(ip_addr), port_num });
            coro_ = connection_.send_continuous_message();
        }

        auto write(const auto& analysis_process)
        {
            auto& executor = app_.get().get_io_context();

            const auto& send_data = analysis_process.get_output_data();
            connection_.set_send_message(send_data);

            return asio::co_spawn(executor, coro_.async_resume(asio::use_awaitable), asio::use_future);
        }

      private:
        using WriteDataType = decltype(std::declval<Deserializers>().get_data<option>());
        UDPWriterConnection connection_;
        std::reference_wrapper<App> app_;
        asio::experimental::coro<int> coro_;
    };

} // namespace srs

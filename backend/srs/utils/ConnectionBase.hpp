#pragma once

#include "CommonDefitions.hpp"
#include "ConnectionTypeDef.hpp"

#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/experimental/coro.hpp>
#include <fmt/ranges.h>
#include <gsl/gsl-lite.hpp>
#include <spdlog/spdlog.h>
#include <srs/Application.hpp>
#include <srs/converters/SerializableBuffer.hpp>
#include <srs/utils/CommonConcepts.hpp>

namespace srs
{

    // derive from enable_shared_from_this to make sure object still alive in the coroutine
    template <int buffer_size = SMALL_READ_MSG_BUFFER_SIZE>
    class ConnectionBase : public std::enable_shared_from_this<ConnectionBase<buffer_size>>
    {
      public:
        explicit ConnectionBase(const ConnectionInfo& info, std::string name)
            : local_port_number_{ info.local_port_number }
            , name_{ std::move(name) }
            , app_{ info.control }
        {
            spdlog::debug("Creating connection {} with buffer size: {}", name_, buffer_size);
        }

        // possible overload from derived class
        void read_data_handle(std::span<BufferElementType> read_data) {}
        void end_of_connection() {}
        void on_fail() { spdlog::debug("default on_fail is called!"); }
        auto get_executor() { return app_->get_io_context().get_executor(); }

        void listen(this auto&& self, bool is_continuous = false);
        void communicate(this auto&& self, const std::vector<CommunicateEntryType>& data, uint16_t address);
        void close_socket();

        auto send_continuous_message() -> asio::experimental::coro<int(std::optional<std::string_view>)>;

        // Settters:
        void set_socket(std::unique_ptr<asio::ip::udp::socket> socket) { socket_ = std::move(socket); }
        void set_remote_endpoint(asio::ip::udp::endpoint endpoint) { endpoint_ = std::move(endpoint); }
        void set_timeout_seconds(int val) { timeout_seconds_ = val; }

        void set_send_message(const RangedData auto& msg)
        {
            continuous_send_msg_ = std::span{ msg.begin(), msg.end() };
        }

        // Getters:
        [[nodiscard]] auto get_read_msg_buffer() const -> const auto& { return read_msg_buffer_; }
        [[nodiscard]] auto get_name() const -> const auto& { return name_; }
        [[nodiscard]] auto get_app() -> auto& { return *app_; }
        auto get_socket() -> const udp::socket& { return *socket_; }
        auto get_endpoint() -> const udp::endpoint& { return endpoint_; }

      private:
        int local_port_number_ = 0;
        uint32_t counter_ = INIT_COUNT_VALUE;
        std::string name_ = "ConnectionBase";
        gsl::not_null<App*> app_;
        std::unique_ptr<udp::socket> socket_;
        udp::endpoint endpoint_;
        SerializableMsgBuffer write_msg_buffer_;
        std::span<const char> continuous_send_msg_;
        ReadBufferType<buffer_size> read_msg_buffer_{};
        int timeout_seconds_ = DEFAULT_TIMEOUT_SECONDS;

        void encode_write_msg(const std::vector<CommunicateEntryType>& data, uint16_t address);
        auto new_shared_socket(int port_number) -> std::unique_ptr<udp::socket>;
        static auto signal_handling(auto* connection) -> asio::awaitable<void>;
        static auto timer_countdown(auto* connection) -> asio::awaitable<void>;
        static auto listen_message(SharedPtr auto connection, bool is_continuous = false) -> asio::awaitable<void>;
        static auto send_message(std::shared_ptr<ConnectionBase> connection) -> asio::awaitable<void>;
        void reset_read_msg_buffer() { std::fill(read_msg_buffer_.begin(), read_msg_buffer_.end(), 0); }
    };

    // Member function definitions:

    template <int size>
    auto ConnectionBase<size>::send_message(std::shared_ptr<ConnectionBase<size>> connection) -> asio::awaitable<void>
    {
        spdlog::debug("Connection {}: Sending data ...", connection->get_name());
        auto data_size = co_await connection->socket_->async_send_to(
            asio::buffer(connection->write_msg_buffer_.data()), connection->endpoint_, asio::use_awaitable);
        spdlog::debug("Connection {}: {} bytes data sent with {:02x}",
                      connection->get_name(),
                      data_size,
                      fmt::join(connection->write_msg_buffer_.data(), " "));
    }

    template <int size>
    auto ConnectionBase<size>::send_continuous_message()
        -> asio::experimental::coro<int(std::optional<std::string_view>)>
    {
        auto send_msg = std::string_view{};
        auto data_size = 0;
        while (true)
        {
            data_size = (not send_msg.empty()) ? socket_->send_to(asio::buffer(send_msg), endpoint_) : 0;
            auto msg = co_yield data_size;

            if (not msg.has_value())
            {
                close_socket();
                co_return;
            }
            else
            {
                send_msg = msg.value();
            }
        }
    }

    template <int size>
    auto ConnectionBase<size>::listen_message(SharedPtr auto connection, bool is_continuous) -> asio::awaitable<void>
    {
        spdlog::debug("Connection {}: starting to listen ...", connection->get_name());
        while (true)
        {
            auto receive_data_size = co_await connection->socket_->async_receive(
                asio::buffer(connection->read_msg_buffer_), asio::use_awaitable);
            auto read_msg = std::span{ connection->read_msg_buffer_.data(), receive_data_size };
            connection->read_data_handle(read_msg);
            // spdlog::info("Connection {}: received {} bytes data", connection->get_name(), read_msg.size());

            connection->reset_read_msg_buffer();
            if (not is_continuous or connection->get_app().get_status().is_on_exit.load())
            {
                break;
            }
        }
        connection->end_of_connection();
    }

    template <int size>
    auto ConnectionBase<size>::timer_countdown(auto* connection) -> asio::awaitable<void>
    {
        auto io_context = co_await asio::this_coro::executor;
        auto timer = asio::steady_timer{ io_context };
        if (connection->timeout_seconds_ > 0)
        {

            timer.expires_after(std::chrono::seconds{ connection->timeout_seconds_ });
        }
        else
        {
            timer.expires_at(decltype(timer)::time_point::max());
        }
        co_await timer.async_wait(asio::use_awaitable);
        spdlog::error("Connection {}: TIMEOUT after {} seconds.", connection->get_name(), connection->timeout_seconds_);
        connection->on_fail();
    }

    template <int size>
    auto ConnectionBase<size>::signal_handling(auto* connection) -> asio::awaitable<void>
    {
        auto interrupt_signal = asio::signal_set(co_await asio::this_coro::executor, SIGINT);
        spdlog::trace("Connection {}: waiting for signals", connection->get_name());
        auto [error, sig_num] = co_await interrupt_signal.async_wait(asio::as_tuple(asio::use_awaitable));
        if (error)
        {
            spdlog::trace("Connection {}: Signal ended with {}", connection->get_name(), error.message());
        }
        else
        {
            fmt::print("\n");
            spdlog::trace("Connection {}: Signal ID {} is called!", connection->get_name(), sig_num);
            connection->end_of_connection();
        }
    }

    template <int size>
    auto ConnectionBase<size>::new_shared_socket(int port_number) -> std::unique_ptr<udp::socket>
    {
        return std::make_unique<udp::socket>(app_->get_io_context(),
                                             udp::endpoint{ udp::v4(), static_cast<asio::ip::port_type>(port_number) });
    }

    template <int size>
    void ConnectionBase<size>::encode_write_msg(const std::vector<CommunicateEntryType>& data, uint16_t address)
    {
        write_msg_buffer_.serialize(
            counter_, ZERO_UINT16_PADDING, address, WRITE_COMMAND_BITS, DEFAULT_TYPE_BITS, COMMAND_LENGTH_BITS);
        write_msg_buffer_.serialize(data);
    }

    template <int size>
    void ConnectionBase<size>::listen(this auto&& self, bool is_continuous)
    {
        using asio::experimental::awaitable_operators::operator||;
        spdlog::debug("Connection {}: creating socket with local port number: {}", self.name_, self.local_port_number_);
        self.socket_ = self.new_shared_socket(self.local_port_number_);

        co_spawn(
            self.app_->get_io_context(),
            signal_handling(&self) || timer_countdown(&self) ||
                listen_message(std::static_pointer_cast<std::remove_cvref_t<decltype(self)>>(self.shared_from_this()),
                               is_continuous),
            asio::detached);
        spdlog::debug("Connection {}: spawned listen coroutine", self.name_);
    }

    template <int size>
    void ConnectionBase<size>::communicate(this auto&& self,
                                           const std::vector<CommunicateEntryType>& data,
                                           uint16_t address)
    {
        self.listen();
        self.encode_write_msg(data, address);
        co_spawn(self.app_->get_io_context(), send_message(self.shared_from_this()), asio::detached);
        spdlog::debug("Connection {}: spawned write coroutine", self.name_);
    }

    template <int size>
    void ConnectionBase<size>::close_socket()
    {
        spdlog::trace("Connection {}: Closing the socket ...", name_);
        socket_->close();
        spdlog::trace("Connection {}: Socket is closed.", name_);
    }
} // namespace srs

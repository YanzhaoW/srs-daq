#pragma once

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/thread_pool.hpp>
// #include <srs/devices/Fec.hpp>
#include <srs/devices/Configuration.hpp>
#include <srs/utils/AppStatus.hpp>
#include <srs/utils/CommonAlias.hpp>
#include <thread>

namespace srs
{
    class DataProcessor;
    class DataReader;

    class App
    {
      public:
        App();

        App(const App&) = delete;
        App(App&&) = delete;
        App& operator=(const App&) = delete;
        App& operator=(App&&) = delete;
        ~App() noexcept;

        // public APIs
        void configure_fec() {}
        void switch_on();
        void switch_off();
        void read_data();

        void notify_status_change() { status_.status_change.notify_all(); }
        void start_analysis();
        void exit();
        void wait_for_finish();
        void wait_for_status(auto&& condition, std::chrono::seconds time_duration = DEFAULT_STATUS_WAITING_TIME_SECONDS)
        {
            status_.wait_for_status(std::forward<decltype(condition)>(condition), time_duration);
        }

        // setters:
        void set_remote_endpoint(std::string_view remote_ip, int port_number);
        void set_fec_data_receiv_port(int port_num) { configurations_.fec_data_receive_port = port_num; }
        void set_status_acq_on(bool val = true) { status_.is_acq_on.store(val); }
        void set_status_acq_off(bool val = true) { status_.is_acq_off.store(val); }
        void set_status_is_reading(bool val = true) { status_.is_reading.store(val); }
        void set_print_mode(DataPrintMode mode);
        void set_output_filenames(const std::vector<std::string>& filenames);
        void set_error_string(std::string_view err_msg) { error_string_ = err_msg; }

        // getters:
        [[nodiscard]] auto get_channel_address() const -> uint16_t { return channel_address_; }
        // [[nodiscard]] auto get_fec_config() const -> const auto& { return fec_config_; }
        [[nodiscard]] auto get_status() const -> const auto& { return status_; }
        [[nodiscard]] auto get_io_context() -> auto& { return io_context_; }
        auto get_data_reader() -> DataReader* { return data_reader_.get(); }
        [[nodiscard]] auto get_error_string() const -> const std::string& { return error_string_; }

      private:
        using udp = asio::ip::udp;

        Status status_;
        uint16_t channel_address_ = DEFAULT_CHANNEL_ADDRE;
        Config configurations_;
        std::string error_string_;
        io_context_type io_context_{ 4 };
        asio::executor_work_guard<io_context_type::executor_type> io_work_guard_;
        asio::signal_set signal_set_{ io_context_, SIGINT, SIGTERM };
        std::jthread working_thread_;
        udp::endpoint remote_endpoint_;
        std::unique_ptr<DataProcessor> data_processor_;
        std::shared_ptr<DataReader> data_reader_;

        void start_work();
        void end_of_work() const;
    };
} // namespace srs

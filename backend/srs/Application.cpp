#include "Application.hpp"
#include "DataProcessor.hpp"
#include "spdlog/spdlog.h"
#include <Connections.hpp>
#include <fmt/ranges.h>
#include <string_view>

namespace srs
{
    App::App()
        : io_work_guard_{ asio::make_work_guard(io_context_) }
    {
        spdlog::set_pattern("[%H:%M:%S] [%^%=7l%$] [thread %t] %v");
        spdlog::info("Welcome to SRS Application");
        data_processor_ = std::make_unique<DataProcessor>(this);
        start_work();
    }

    App::~App() = default;

    void App::start_work()
    {
        auto monitoring_action = [this]()
        {
            signal_set_.async_wait(
                [this](const boost::system::error_code& error, auto)
                {
                    if (error == asio::error::operation_aborted)
                    {
                        return;
                    }

                    spdlog::trace("calling SIGINT from monitoring thread");
                    exit();
                });
            io_context_.join();
            end_of_work();
        };
        working_thread_ = std::jthread{ monitoring_action };
    }

    void App::end_of_work()
    {
        if (not status_.is_acq_off.load())
        {
            spdlog::critical(
                "Failed to close srs system! Please manually close the system with\n\nsrs_control --acq-off\n");
        }
    }

    void App::exit()
    {
        signal_set_.cancel();
        data_processor_->stop();
        spdlog::debug("Shutting down application ...");
        status_.is_on_exit.store(true);
        status_.wait_for_status([](const auto& status) { return not status.is_reading.load(); });

        if (status_.is_acq_on.load())
        {
            spdlog::debug("Turning srs system off ...");
            switch_off();
        }
        else
        {
            set_status_acq_off(true);
        }
        set_status_acq_on(false);
        io_work_guard_.reset();
    }

    void App::set_print_mode(DataPrintMode mode) { data_processor_->set_print_mode(mode); }
    void App::set_output_filenames(std::vector<std::string> filenames)
    {
        data_processor_->set_output_filenames(std::move(filenames));
    }

    void App::set_remote_endpoint(std::string_view remote_ip, int port_number)
    {
        auto resolver = udp::resolver{ io_context_ };
        spdlog::debug("Set the remote socket with ip: {} and port: {}", remote_ip, port_number);
        auto udp_endpoints = resolver.resolve(udp::v4(), remote_ip, fmt::format("{}", port_number));
        remote_endpoint_ = *udp_endpoints.begin();
    }

    void App::switch_on()
    {
        auto connection_info = ConnectionInfo{ this };
        connection_info.local_port_number = FEC_CONTROL_LOCAL_PORT;
        auto connection = std::make_shared<Starter>(connection_info);
        connection->set_remote_endpoint(remote_endpoint_);
        connection->acq_on();
    }

    void App::switch_off()
    {
        auto connection_info = ConnectionInfo{ this };
        connection_info.local_port_number = FEC_CONTROL_LOCAL_PORT;
        auto connection = std::make_shared<Stopper>(connection_info);
        connection->set_remote_endpoint(remote_endpoint_);
        connection->acq_off();
    }

    void App::read_data()
    {
        auto connection_info = ConnectionInfo{ this };
        connection_info.local_port_number = FEC_DAQ_RECEIVE_PORT;
        auto data_reader = std::make_shared<DataReader>(connection_info, data_processor_.get());
        data_reader->start();
    }

    void App::run()
    {
        data_processor_->start();
        working_thread_.join();
    }
} // namespace srs

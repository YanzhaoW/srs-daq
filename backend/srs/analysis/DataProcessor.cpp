#include "DataProcessor.hpp"
#include "DataStructs.hpp"
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/ranges.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <srs/Application.hpp>

namespace srs
{
    DataMonitor::DataMonitor(DataProcessor* processor, io_context_type* io_context)
        : processor_{ processor }
        , io_context_{ io_context }
        , clock_{ *io_context_ }
    {
    }

    auto DataMonitor::print_cycle() -> asio::awaitable<void>
    {
        clock_.expires_after(period_);
        console_ = spdlog::stdout_color_st("Data Monitor");
        auto console_format = std::make_unique<spdlog::pattern_formatter>(
            "[%H:%M:%S] <%n> %v", spdlog::pattern_time_type::local, std::string(""));
        console_->set_formatter(std::move(console_format));
        console_->flush_on(spdlog::level::info);
        console_->set_level(spdlog::level::info);
        while (true)
        {
            co_await clock_.async_wait(asio::use_awaitable);
            clock_.expires_after(period_);

            auto total_bytes_count = processor_->get_read_data_bytes();
            auto total_hits_count = processor_->get_processed_hit_number();

            auto time_now = std::chrono::steady_clock::now();

            auto time_duration = std::chrono::duration_cast<std::chrono::microseconds>(time_now - last_print_time_);
            auto bytes_read = static_cast<double>(total_bytes_count - last_read_data_bytes_);
            auto hits_processed = static_cast<double>(total_hits_count - last_processed_hit_num_);

            last_read_data_bytes_ = total_bytes_count;
            last_processed_hit_num_ = total_hits_count;
            last_print_time_ = time_now;

            const auto time_duration_double = static_cast<double>(time_duration.count());
            current_received_bytes_MBps_.store(bytes_read / time_duration_double);
            current_hits_ps_.store(hits_processed / time_duration_double * 1e6);

            set_speed_string(current_received_bytes_MBps_.load());
            console_->info("Data reading rate: {}. Press \"Ctrl-C\" to stop the program.\r", speed_string_);
        }
    }

    void DataMonitor::set_speed_string(double speed_MBps)
    {
        if (speed_MBps < 1.)
        {
            speed_string_ =
                fmt::format(fg(fmt::color::yellow) | fmt::emphasis::bold, "{:>7.5} KB/s", 1000. * speed_MBps);
        }
        else
        {
            speed_string_ = fmt::format(fg(fmt::color::yellow) | fmt::emphasis::bold, "{:>7.5} MB/s", speed_MBps);
        }
    }

    void DataMonitor::start() { asio::co_spawn(*io_context_, print_cycle(), asio::detached); }

    void DataMonitor::stop()
    {
        clock_.cancel();
        spdlog::debug("DataMonitor: rate polling clock is killed.");
    }

    DataProcessor::DataProcessor(App* control)
        : control_{ control }
        , monitor_{ this, &control_->get_io_context() }
    {
    }

    void DataProcessor::start()
    {
        is_stopped.store(false);
        if (print_mode_ == print_speed)
        {
            monitor_.start();
        }
        asio::post(control_->get_io_context(), [this]() { analysis_loop(); });
    }

    void DataProcessor::stop()
    {
        // CAS operation to guarantee the thread safty
        auto expected = false;
        if (is_stopped.compare_exchange_strong(expected, true))
        {
            monitor_.stop();
            data_queue_.abort();
        }
    }

    void DataProcessor::read_data_once(std::span<BufferElementType> read_data)
    {
        total_read_data_bytes_ += read_data.size();
        auto is_success = data_queue_.try_emplace(read_data);
        if (not is_success)
        {
            spdlog::critical("Data queue is full and message is lost. Try to increase its capacity!");
        }
    }

    void DataProcessor::analysis_loop()
    {
        try
        {
            spdlog::trace("entering analysis loop");
            // TODO: Use direct binary data

            while (not is_stopped)
            {
                data_queue_.pop(deserializers_.get_binary_ref());
                analyse_one_frame();
                print_data();
                write_data();

                deserializers_.clear();
            }
        }
        catch (oneapi::tbb::user_abort& ex)
        {
            spdlog::debug("Data processing: {}", ex.what());
        }
        catch (std::exception& ex)
        {
            spdlog::critical(ex.what());
            control_->exit();
        }
    }

    void DataProcessor::analyse_one_frame()
    {
        const auto& bin_data = deserializers_.get_data<Deserializers::binary>().data();
        struct_serializer.convert(bin_data);
        const auto& export_data = struct_serializer.get_output_data();

        total_processed_hit_numer_ += export_data.hit_data.size();
        monitor_.update(export_data);
        if (print_mode_ == print_raw)
        {
            spdlog::info("data: {:x}", fmt::join(bin_data, ""));
        }
    }

    void DataProcessor::write_data()
    {
        const auto& bin_data = deserializers_.get_data<Deserializers::binary>().data();
        const auto& export_data = struct_serializer.get_output_data();
        if (data_writer_.has_binary())
        {
            data_writer_.write_binary(bin_data);
        }

        if (data_writer_.has_struct())
        {

            auto& export_data_nonconst = struct_serializer.get_output_data_ref();
            data_writer_.write_struct(export_data_nonconst);
        }
    }

    void DataProcessor::print_data()
    {
        const auto& export_data = struct_serializer.get_output_data();
        if (print_mode_ == print_header or print_mode_ == print_raw or print_mode_ == print_all)
        {
            spdlog::info("frame header: [ {} ]. Data size: {}", export_data.header, received_data_size_);
        }

        if (print_mode_ == print_all)
        {
            for (const auto& hit_data : export_data.hit_data)
            {
                spdlog::info("Hit data: [ {} ]", hit_data);
            }
            for (const auto& marker_data : export_data.marker_data)
            {
                spdlog::info("Marker data: [ {} ]", marker_data);
            }
        }
    }
} // namespace srs

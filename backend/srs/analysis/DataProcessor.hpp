#pragma once

#include "CommonDefitions.hpp"
#include "DataStructs.hpp"
#include "DataWriter.hpp"
#include <asio/awaitable.hpp>
#include <asio/steady_timer.hpp>
#include <atomic>
#include <chrono>
#include <gsl/gsl-lite.hpp>
#include <span>
#include <spdlog/logger.h>
#include <srs/serializers/SerializableBuffer.hpp>
#include <srs/serializers/StructDeserializer.hpp>
#include <tbb/concurrent_queue.h>

namespace srs
{
    class DataProcessor;
    class App;

    class DataMonitor
    {
      public:
        explicit DataMonitor(DataProcessor* processor, io_context_type* io_context);
        void show_data_speed(bool val = true) { is_shown_ = val; }
        void set_display_period(std::chrono::milliseconds duration) { period_ = duration; }
        void start();
        void stop();
        void update(const StructData& data_struct) {}
        void http_server_loop();

        // getters:
        [[nodiscard]] auto get_received_bytes_MBps() const -> double { return current_received_bytes_MBps_.load(); }
        [[nodiscard]] auto get_processed_hit_rate() const -> double { return current_hits_ps_.load(); }

      private:
        bool is_shown_ = true;
        gsl::not_null<DataProcessor*> processor_;
        gsl::not_null<io_context_type*> io_context_;
        std::shared_ptr<spdlog::logger> console_;
        asio::steady_timer clock_;
        std::atomic<uint64_t> last_read_data_bytes_ = 0;
        std::atomic<uint64_t> last_processed_hit_num_ = 0;
        std::chrono::time_point<std::chrono::steady_clock> last_print_time_ = std::chrono::steady_clock::now();
        std::chrono::milliseconds period_ = DEFAULT_DISPLAY_PERIOD;
        std::atomic<double> current_received_bytes_MBps_;
        std::atomic<double> current_hits_ps_;
        std::string speed_string_;

        void set_speed_string(double speed_MBps);
        auto print_cycle() -> asio::awaitable<void>;
    };

    class DataProcessor
    {
      public:
        explicit DataProcessor(App* control);

        // From socket interface. Need to be fast return
        void read_data_once(std::span<BufferElementType> read_data);

        void start();
        void stop();

        // getters:
        [[nodiscard]] auto get_read_data_bytes() const -> uint64_t { return total_read_data_bytes_.load(); }
        [[nodiscard]] auto get_processed_hit_number() const -> uint64_t { return total_processed_hit_numer_.load(); }
        [[nodiscard]] auto get_export_data() -> auto& { return export_data_; }
        [[nodiscard]] auto get_data_monitor() const -> const auto& { return monitor_; }

        // setters:
        void set_print_mode(DataPrintMode mode) { print_mode_ = mode; }
        void set_show_data_speed(bool val = true) { monitor_.show_data_speed(val); }
        void set_monitor_display_period(std::chrono::milliseconds duration) { monitor_.set_display_period(duration); }
        void set_output_filenames(std::vector<std::string> filenames)
        {
            data_writer_.set_output_filenames(std::move(filenames));
        }

      private:
        using enum DataPrintMode;

        std::atomic<bool> is_stopped{ false };
        std::size_t received_data_size_ {};
        DataPrintMode print_mode_ = DataPrintMode::print_speed;
        tbb::concurrent_bounded_queue<SerializableMsgBuffer> data_queue_;
        std::atomic<uint64_t> total_read_data_bytes_ = 0;
        std::atomic<uint64_t> total_processed_hit_numer_ = 0;
        gsl::not_null<App*> control_;
        DataWriter data_writer_{ this };
        DataMonitor monitor_;

        // buffer variables
        StructDeserializer struct_serializer;
        // ReceiveDataSquence receive_raw_data_;
        StructData export_data_;

        // should run on a different task
        void analysis_loop();
        // void translate_raw_data(const ReceiveDataSquence& data_seq);
        void analyse_one_frame(const SerializableMsgBuffer& a_frame);
        void write_data(const SerializableMsgBuffer& a_frame);
        void print_data();
        void clear_data_buffer();
        // static bool check_is_hit(const DataElementType& element);
    };

} // namespace srs

#pragma once

#ifdef HAS_ROOT

#include "DataStructs.hpp"
#include <TCanvas.h>
#include <TGraph.h>
#include <THttpServer.h>
#include <TNamed.h>
#include <spdlog/spdlog.h>

namespace srs
{
    class DataMonitor;
    class RootHttpServer : public TNamed
    {
      public:
        RootHttpServer(const RootHttpServer&) = delete;
        RootHttpServer(RootHttpServer&&) = delete;
        RootHttpServer& operator=(const RootHttpServer&) = delete;
        RootHttpServer& operator=(RootHttpServer&&) = delete;
        explicit RootHttpServer(DataMonitor* processor);
        ~RootHttpServer() { spdlog::info("Root http server is killed"); }

        void update(const ExportData& data_struct);
        void process_request();
        void terminate() { http_server_.SetTerminate(); }
        void reset_data_rate_range();

      private:
        int max_num_data_point_ = 400;
        int data_point_index_ = 0;
        DataMonitor* data_monitor_ = nullptr;
        THttpServer http_server_{ "http:8080?loopback" };
        std::unique_ptr<TCanvas> data_rate_canvas_;
        std::unique_ptr<TCanvas> data_oscilloscope_;
        std::unique_ptr<TGraph> hit_rate_graph_;
        std::pair<double, double> hit_rate_y_range_;
        std::unique_ptr<TGraph> byte_rate_graph_;
        std::pair<double, double> byte_rate_y_range_;
        std::chrono::time_point<std::chrono::steady_clock> last_cycle_begin_time_;

        std::mutex mutex_;

        void init();
        void reset();
        void draw();
        void polling();
        void poll_data_rate(int index);
        void do_register();
        void set_plots();

        ClassDefOverride(RootHttpServer, 1);
    };

} // namespace srs

#endif

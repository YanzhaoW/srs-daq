#ifdef HAS_ROOT

#include "RootHttpServer.hpp"
#include "DataProcessor.hpp"
#include <TFrame.h>
#include <TH1.h>

namespace srs
{
    namespace
    {
        constexpr auto padding_frac = 0.2;
        void set_padded_range(auto* graph, const std::pair<double, double>& range)
        {
            if ((range.second - range.first) == 0.)
            {
                return;
            }
            auto padding = (range.second - range.first) * padding_frac;
            graph->GetYaxis()->SetRangeUser(range.first - padding, range.second + padding);
        }

        void adjust_range(std::pair<double, double>& range, double value)
        {
            range.first = (range.first == 0) or (range.first > value) ? value : range.first;
            range.second = (range.second == 0) or (range.second < value) ? value : range.second;
        }

    } // namespace

    RootHttpServer::RootHttpServer(DataMonitor* processor)
        : TNamed{ "http_server", "Http server for srs system" }
        , data_monitor_{ processor }
    {
        http_server_.SetReadOnly(false);
        http_server_.SetTimer(0, kTRUE);
        init();
    }

    void RootHttpServer::init()
    {
        TH1::AddDirectory(false);
        data_rate_canvas_ = std::make_unique<TCanvas>("data_rate", "Data reading rate", 0, 0, 900, 600);
        data_rate_canvas_->Divide(1, 2);

        set_plots();
        do_register();
    }

    void RootHttpServer::set_plots()
    {
        hit_rate_graph_ = std::make_unique<TGraph>(max_num_data_point_);
        hit_rate_graph_->SetName("hit_rate");
        hit_rate_graph_->GetXaxis()->SetTitle("Time (seconds)");
        hit_rate_graph_->GetYaxis()->SetTitle("Processed hit rate [/s]");
        hit_rate_graph_->SetTitle("Number of processed hits per second");
        hit_rate_graph_->Expand(max_num_data_point_);
        set_padded_range(hit_rate_graph_.get(), hit_rate_y_range_);

        byte_rate_graph_ = std::make_unique<TGraph>(max_num_data_point_);
        byte_rate_graph_->SetName("byte_rate");
        byte_rate_graph_->GetXaxis()->SetTitle("Time (seconds)");
        byte_rate_graph_->GetYaxis()->SetTitle("Received byte rate [MB/s]");
        byte_rate_graph_->SetTitle("Received bytes per second");
        byte_rate_graph_->Expand(max_num_data_point_);
        set_padded_range(byte_rate_graph_.get(), byte_rate_y_range_);
    }

    void RootHttpServer::update(const ExportData& data_struct) {}

    void RootHttpServer::draw()
    {
        data_rate_canvas_->cd(1);
        hit_rate_graph_->Draw();
        data_rate_canvas_->cd(2);
        byte_rate_graph_->Draw();
        data_rate_canvas_->GetPad(1)->ModifiedUpdate();
        data_rate_canvas_->GetPad(2)->ModifiedUpdate();
        data_rate_canvas_->GetPad(1)->RangeAxisChanged();
        data_rate_canvas_->GetPad(2)->RangeAxisChanged();
        data_rate_canvas_->ModifiedUpdate();
        data_rate_canvas_->RangeChanged();
        data_rate_canvas_->Draw();
    }

    void RootHttpServer::process_request()
    {
        auto lock_guard = std::lock_guard{ mutex_ };
        polling();
        draw();
        http_server_.ProcessRequests();
    }

    void RootHttpServer::polling()
    {
        if (data_point_index_ == 0)
        {
            last_cycle_begin_time_ = std::chrono::steady_clock::now();
        }

        poll_data_rate(data_point_index_);

        if (++data_point_index_ == max_num_data_point_)
        {
            data_point_index_ = 0;
        }
    }

    void RootHttpServer::reset_data_rate_range()
    {
        auto lock_guard = std::lock_guard{ mutex_ };
        data_point_index_ = 0;
        set_plots();

        data_rate_canvas_->Clear();
    }

    void RootHttpServer::poll_data_rate(int index)
    {

        const auto byte_reading_rate_MBps = data_monitor_->get_received_bytes_MBps();
        const auto hits_rate = data_monitor_->get_processed_hit_rate();

        if (byte_reading_rate_MBps == 0. or hits_rate == 0)
        {
            return;
        }

        const auto time_elapsed_milli_sec =
            static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() -
                                                                                      last_cycle_begin_time_)
                                    .count());

        adjust_range(byte_rate_y_range_, byte_reading_rate_MBps);
        adjust_range(hit_rate_y_range_, hits_rate);

        hit_rate_graph_->SetPoint(index, time_elapsed_milli_sec * 1e-3, hits_rate);
        hit_rate_graph_->Set(index + 1);
        byte_rate_graph_->SetPoint(data_point_index_, time_elapsed_milli_sec * 1e-3, byte_reading_rate_MBps);
        byte_rate_graph_->Set(index + 1);
    }

    void RootHttpServer::do_register()
    {
        auto get_register_command = [this](std::string_view member_function)
        { return fmt::format(R"(/Control/{}/->{}())", GetName(), member_function); };

        // TODO: this somehow doesn't work. It hangs the root http server.

        // http_server_.Register("graphs/data_rate", data_rate_canvas_.get());
        // http_server_.Register("/Control", this);
        // http_server_.RegisterCommand("/Control/reset_rate_range",
        //                              get_register_command("reset_data_rate_range").c_str());
    }
} // namespace srs

#endif

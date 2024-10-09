#pragma once
#include <atomic>
#include <condition_variable>
#include <srs/utils/CommonDefitions.hpp>

namespace srs
{
    struct Status
    {
        std::atomic<bool> is_configured = false;
        std::atomic<bool> is_acq_on = false;
        std::atomic<bool> is_reading = false;
        std::atomic<bool> is_acq_off = false;
        std::condition_variable status_change;

        void wait_for_status(auto&& condition, std::chrono::seconds time_duration = DEFAULT_STATUS_WAITING_TIME_SECONDS)
        {
            auto mutex = std::mutex{};
            while (not condition(*this))
            {
                auto lock = std::unique_lock<std::mutex>{ mutex };
                auto res = status_change.wait_for(lock, time_duration);
                if (res == std::cv_status::timeout)
                {
                    throw std::runtime_error("TIMEOUT");
                }
            }
        }
    };

}; // namespace srs

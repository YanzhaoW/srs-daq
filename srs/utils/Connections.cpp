#include "Connections.hpp"
#include <DataProcessor.hpp>

namespace srs
{
    void Starter::end_of_read()
    {
        close_socket();
        auto& control = get_control();
        control.set_status_acq_on();
        control.notify_status_change();
        spdlog::info("SRS system is turned on");
    }

    void Stopper::acq_off()
    {
        const auto waiting_time = std::chrono::seconds{ 4 };
        get_control().wait_for_status([](const Status& status) { return status.is_acq_on.load(); }, waiting_time);
        const auto data = std::vector<CommunicateEntryType>{ 0, 15, 0 };
        communicate(data, NULL_ADDRESS);
    }

    void DataReader::end_of_read()
    {
        spdlog::debug("Stopping data reading ...");
        close_socket();
        auto& control = get_control();
        control.set_status_is_reading(false);
        control.notify_status_change();
        spdlog::info("Data reading is stopped.");
    }

    void DataReader::read_data_handle(std::span<BufferElementType> read_data)
    {
        data_processor_->read_data_once(read_data);
    }
} // namespace srs

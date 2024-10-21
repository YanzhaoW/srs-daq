#pragma once

#include "ConnectionBase.hpp"

namespace srs
{

    class DataProcessor;

    class Starter : public ConnectionBase<>
    {
      public:
        explicit Starter(const ConnectionInfo& info)
            : ConnectionBase(info, "Starter")
        {
        }

        void end_of_connection();
        void acq_on()
        {
            const auto data = std::vector<CommunicateEntryType>{ 0, 15, 1 };
            communicate(data, NULL_ADDRESS);
        }
        void on_fail()
        {
            const auto& endpoint = get_endpoint();
            spdlog::critical(
                "Cannot start the system: No connection to {}:{}!", endpoint.address().to_string(), endpoint.port());
        }
    };

    class Stopper : public ConnectionBase<>
    {
      public:
        Stopper(const Stopper&) = delete;
        Stopper(Stopper&&) = default;
        Stopper& operator=(const Stopper&) = delete;
        Stopper& operator=(Stopper&&) = default;

        explicit Stopper(const ConnectionInfo& info)
            : ConnectionBase(info, "Stopper")
        {
        }

        ~Stopper()
        {
            spdlog::info("SRS system is turned off");
            get_app().set_status_acq_off();
        }

        static void on_fail() { spdlog::debug("on_fail of stopper is called"); }
        void acq_off();
        void end_of_connection() {}
    };

    class DataReader : public ConnectionBase<LARGE_READ_MSG_BUFFER_SIZE>
    {
      public:
        DataReader(const ConnectionInfo& info, DataProcessor* processor)
            : data_processor_{ processor }
            , ConnectionBase(info, "DataReader")
        {
            set_timeout_seconds(-1);
        }

        void start()
        {
            get_app().set_status_is_reading(true);
            listen(true);
        }
        void end_of_connection();

        void read_data_handle(std::span<BufferElementType> read_data);

      private:
        gsl::not_null<DataProcessor*> data_processor_;
    };

} // namespace srs

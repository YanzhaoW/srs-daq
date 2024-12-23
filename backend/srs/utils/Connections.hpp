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

        void close();
        void acq_on()
        {
            const auto data = std::vector<CommunicateEntryType>{ 0, 15, 1 };
            communicate(data, NULL_ADDRESS);
        }
        void on_fail()
        {
            const auto& endpoint = get_remote_endpoint();
            spdlog::critical(
                "Cannot start the system: No connection to {}:{}!", endpoint.address().to_string(), endpoint.port());
        }
    };

    class Stopper : public ConnectionBase<>
    {
      public:
        Stopper(const Stopper&) = delete;
        Stopper(Stopper&&) = delete;
        Stopper& operator=(const Stopper&) = delete;
        Stopper& operator=(Stopper&&) = delete;

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
        // void close() {}
    };

    class DataReader : public ConnectionBase<LARGE_READ_MSG_BUFFER_SIZE>
    {
      public:
        DataReader(const ConnectionInfo& info, DataProcessor* processor)
            : data_processor_{ processor }
            , ConnectionBase(info, "DataReader")
        {
            set_timeout_seconds(1);
            set_continuous();
        }

        DataReader(const DataReader&) = delete;
        DataReader(DataReader&&) = delete;
        DataReader& operator=(const DataReader&) = delete;
        DataReader& operator=(DataReader&&) = delete;
        ~DataReader() = default;

        void start(bool is_non_stop = true)
        {
            set_socket(new_shared_socket(get_local_port_number()));
            const auto& is_on_exit = get_app().get_status().is_on_exit;
            if (not is_on_exit.load())
            {
                get_app().set_status_is_reading(true);
                listen(is_non_stop);
            }
            else
            {
                spdlog::debug("Program is alreay on exit!");
            }
        }
        void close();

        void read_data_handle(std::span<BufferElementType> read_data);

      private:
        gsl::not_null<DataProcessor*> data_processor_;
    };

} // namespace srs

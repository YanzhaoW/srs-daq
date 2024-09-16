#pragma once

#include "VmmDevice.hpp"
#include <string>

namespace srs
{
    struct HybridDevice
    {
        std::vector<vmm::Device> vmms;
    };

    struct I2CDevice
    {
        uint8_t address{};
        int sequence{};
        int data{};
        int reg{};
        uint32_t result{};
    };

    struct UDPSocket
    {
    };

    namespace fec
    {
        struct DeviceConnection
        {
            std::string ip_addr;
            int port{};
            int daq_port{};
        };

        enum class State
        {
            invalid,
            fresh,
            configured
        };

        struct Config
        {
            uint8_t clock_source{};
            uint32_t debug_data_format{};
            uint32_t latency_reset{};
            uint32_t latency_data_max{};
            uint32_t latency_data_error{};
            uint32_t tp_offset_first{};
            uint32_t tp_offset{};
            uint32_t tp_latency{};
            uint32_t tp_number{};
            uint32_t tp_skew{};
            uint32_t tp_width{};
            uint32_t tp_polarity{};
            uint32_t ckbc_skew{};
            uint32_t ckbc{};
            uint32_t ckdt{};
            int debug{};
            int break_on_pkt_cnt_mismatch{};
            DeviceConnection connection;
        };

        class Devices
        {
          public:
            Devices() = default;

          private:
            std::unique_ptr<UDPSocket> socket;
            uint32_t packet_counter{};
            uint8_t channel_map{};
            // uint8_t n_hybrids{};
            uint8_t hybrid_index{};
            uint8_t vmm_index{};
            uint8_t adc_channel{};
            uint8_t id{}; /* made from lowest 8 bits of IP address */
            std::vector<uint8_t> hybrid_index_map;
            State state{};
            std::vector<HybridDevice> hybrid;
            Config config{};
            I2CDevice i2c{};
        };
    }; // namespace fec
} // namespace srs

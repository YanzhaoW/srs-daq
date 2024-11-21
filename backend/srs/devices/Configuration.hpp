#pragma once

#include <srs/utils/CommonDefitions.hpp>

namespace srs
{
    struct Config
    {
        int fec_control_local_port = FEC_CONTROL_LOCAL_PORT;
        int fec_data_receive_port = FEC_DAQ_RECEIVE_PORT;
    };
} // namespace srs
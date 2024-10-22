#pragma once

#include "message.pb.h"
#include <srs/serializers/StructDeserializer.hpp>

namespace srs
{
    class Struct2ProtoConverter
    {
      public:
        explicit Struct2ProtoConverter() = default;

        using InputType = StructData;
        using OutputType = proto::Data;

        auto convert(const InputType& struct_data) -> const OutputType&
        {
            set_header(struct_data);
            set_marker_data(struct_data);
            set_hit_data(struct_data);
            return output_data_;
        }

        // void reset() { output_data_.clear(); }
        [[nodiscard]] auto get_output_data() const -> const auto& { return output_data_; }

      private:
        proto::StructHeader header_buffer_;
        OutputType output_data_;

        void set_header(const InputType& struct_data)
        {
            const auto& input_header = struct_data.header;
            header_buffer_.set_frame_counter(input_header.frame_counter);
            header_buffer_.set_fec_id(input_header.fec_id);
            header_buffer_.set_udp_timestamp(input_header.udp_timestamp);
            header_buffer_.set_overflow(input_header.overflow);
            output_data_.set_allocated_header(&header_buffer_);
        }

        void set_marker_data(const InputType& struct_data)
        {
            const auto& input_marker_data = struct_data.marker_data;
            for (const auto& input_data : input_marker_data)
            {
                auto* marker_data = output_data_.add_marker_data();
                marker_data->set_vmm_id(input_data.vmm_id);
                marker_data->set_srs_timestamp(input_data.srs_timestamp);
            }
        }

        void set_hit_data(const InputType& struct_data)
        {
            const auto& input_hit_data = struct_data.hit_data;
            for (const auto& input_data : input_hit_data)
            {
                auto* hit_data = output_data_.add_hit_data();
                hit_data->set_is_over_threshold(input_data.is_over_threshold);
                hit_data->set_channel_num(input_data.channel_num);
                hit_data->set_tdc(input_data.tdc);
                hit_data->set_offset(input_data.offset);
                hit_data->set_vmm_id(input_data.vmm_id);
                hit_data->set_adc(input_data.adc);
                hit_data->set_bc_id(input_data.bc_id);
            }
        }
    };
} // namespace srs

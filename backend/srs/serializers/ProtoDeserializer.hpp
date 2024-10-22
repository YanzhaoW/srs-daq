#pragma once

#include "message.pb.h"
#include <srs/serializers/StructDeserializer.hpp>

namespace srs
{
    class ProtoDeserializer
    {
      public:
        explicit ProtoDeserializer() = default;

        using InputType = proto::Data;
        using OutputType = std::string;

        auto convert(const InputType& proto_data) -> std::size_t
        {
            namespace protobuf = google::protobuf;
            namespace io = protobuf::io;
            auto output_stream = io::StringOutputStream{ &output_data_ };
            proto_data.SerializeToZeroCopyStream(&output_stream);
            return 0;
        }

        void reset() { output_data_.clear(); }
        [[nodiscard]] auto get_output_data() const -> const auto& { return output_data_; }

      private:
        OutputType output_data_;
    };
} // namespace srs

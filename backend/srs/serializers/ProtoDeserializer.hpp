#pragma once

#include <srs/serializers/ProtoDeserializerBase.hpp>

namespace srs
{
    const auto protobuf_deserializer_converter = [](const proto::Data& proto_data, std::string& output_data) -> int
    {
        namespace protobuf = google::protobuf;
        namespace io = protobuf::io;
        auto output_stream = io::StringOutputStream{ &output_data };
        proto_data.SerializeToZeroCopyStream(&output_stream);
        return 0;
    };

    class ProtoDeserializer : public ProtoDeserializerBase<decltype(protobuf_deserializer_converter)>
    {
      public:
        explicit ProtoDeserializer(asio::thread_pool& thread_pool)
            : ProtoDeserializerBase{ thread_pool, protobuf_deserializer_converter }
        {
        }
    };
} // namespace srs

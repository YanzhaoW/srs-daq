#pragma once

#include <google/protobuf/util/delimited_message_util.h>
#include <srs/serializers/ProtoDeserializerBase.hpp>

namespace srs
{
    const auto protobuf_delim_deserializer_converter = [](const proto::Data& proto_data,
                                                          std::string& output_data) -> int
    {
        namespace protobuf = google::protobuf;
        namespace io = protobuf::io;
        auto output_stream = io::StringOutputStream{ &output_data };
        protobuf::util::SerializeDelimitedToZeroCopyStream(proto_data, &output_stream);
        return 0;
    };

    class ProtoDelimDeserializer : public ProtoDeserializerBase<decltype(protobuf_delim_deserializer_converter)>
    {
      public:
        explicit ProtoDelimDeserializer(asio::thread_pool& thread_pool)
            : ProtoDeserializerBase{ thread_pool, protobuf_delim_deserializer_converter }
        {
        }
    };
} // namespace srs

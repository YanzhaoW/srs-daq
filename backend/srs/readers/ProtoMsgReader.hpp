#pragma once

#include <memory>
#include <srs/data/DataStructs.hpp>
#include <string_view>

namespace srs
{
    class Proto2StructConverter;
    class ProtoDeserializer;
    class ProtoMsgReader
    {
      public:
        ProtoMsgReader();

        ProtoMsgReader(const ProtoMsgReader&) = delete;
        ProtoMsgReader(ProtoMsgReader&&) = default;
        ProtoMsgReader& operator=(const ProtoMsgReader&) = delete;
        ProtoMsgReader& operator=(ProtoMsgReader&&) = default;
        ~ProtoMsgReader();

        auto convert(std::string_view msg, StructData& struct_data);
        auto convert(std::string_view msg) -> const StructData&;

      private:
        std::unique_ptr<Proto2StructConverter> proto_to_struct_converter_;
        std::unique_ptr<ProtoDeserializer> proto_deserializer_;
    };
} // namespace srs

#pragma once

#include <srs/serializers/StructDeserializer.hpp>

namespace srs
{
    class ProtobufSerializer
    {
      public:
        explicit ProtobufSerializer() = default;

        using OutputType = StructData;

        auto convert(const BinaryData& binary_data, StructData& struct_data) -> std::size_t
        {
        }

      private:
    };

} // namespace srs

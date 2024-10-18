#pragma once

#include <srs/serializers/StructDeserializer.hpp>

namespace srs
{
    class ProtoDeserializer
    {
      public:
        explicit ProtoDeserializer() = default;

        using InputType = StructData;
        using OutputType = std::string;

        auto convert(const InputType& struct_data, OutputType& proto_data) -> std::size_t { return 0; }

      private:
    };

} // namespace srs

#pragma once

#include <spdlog/spdlog.h>
#include <srs/analysis/DataStructs.hpp>
#include <srs/serializers/DataDeserializeOptions.hpp>
#include <srs/serializers/ProtoDeserializer.hpp>
#include <srs/serializers/SerializableBuffer.hpp>
#include <srs/serializers/StructDeserializer.hpp>
#include <srs/serializers/StructToProtoConverter.hpp>
#include <srs/utils/ValidData.hpp>

namespace srs
{
    class Deserializers
    {
      public:
        Deserializers() = default;
        using enum DataDeserializeOptions;

        void analysis_one(auto& data_queue)
        {
            data_queue.pop(binary_data_);
            struct_deserializer_.convert(binary_data_.data());
            struct_proto_converter_.convert(struct_deserializer_.get_output_data());
            proto_deserializer_.convert(struct_proto_converter_.get_output_data());
        }

        // Getters:
        template <DataDeserializeOptions option>
        auto get_data() -> const auto&;

        void clear()
        {
            struct_deserializer_.reset();
            proto_deserializer_.reset();
        }

      private:
        SerializableMsgBuffer binary_data_;
        StructDeserializer struct_deserializer_;
        Struct2ProtoConverter struct_proto_converter_;
        ProtoDeserializer proto_deserializer_;
    };

    template <DataDeserializeOptions option>
    auto Deserializers::get_data() -> const auto&
    {
        if constexpr (option == binary)
        {
            return binary_data_.data();
        }
        else if constexpr (option == structure)
        {
            return struct_deserializer_.get_output_data();
        }
        else if constexpr (option == proto)
        {
            return proto_deserializer_.get_output_data();
        }
        else
        {
            static_assert(false, "Cannot get the data from this option!");
        }
    };
} // namespace srs

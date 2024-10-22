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

        void deserialize(DataDeserializeOptions option)
        {
            deserialize_struct();
            convert_to_proto();
            convert_to_string();
        }

        // Getters:
        template <DataDeserializeOptions option>
        auto get_data() -> const auto&;
        auto get_binary_ref() -> auto&
        {
            if (binary_data_.is_valid())
            {
                spdlog::warn("Binary data is already copied");
            }
            binary_data_.validate();
            return binary_data_.value();
        }

        void clear()
        {
            if (binary_data_.is_valid())
            {
                binary_data_->clear();
                binary_data_.invalidate();
            }
            struct_deserializer_.reset();
            proto_deserializer_.reset();
        }

      private:
        ValidData<SerializableMsgBuffer> binary_data_;
        StructDeserializer struct_deserializer_;
        Struct2ProtoConverter struct_proto_converter_;
        ProtoDeserializer proto_deserializer_;

        void deserialize_struct() { struct_deserializer_.convert(binary_data_->data()); }

        void convert_to_proto() { struct_proto_converter_.convert(struct_deserializer_.get_output_data()); }

        void convert_to_string() { proto_deserializer_.convert(struct_proto_converter_.get_output_data()); }
    };

    template <DataDeserializeOptions option>
    auto Deserializers::get_data() -> const auto&
    {
        if constexpr (option == binary)
        {
            return binary_data_.get();
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

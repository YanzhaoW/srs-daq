#pragma once

#include <spdlog/spdlog.h>
#include <srs/analysis/DataStructs.hpp>
#include <srs/serializers/DataDeserializeOptions.hpp>
#include <srs/serializers/ProtoDeserializer.hpp>
#include <srs/serializers/SerializableBuffer.hpp>
#include <srs/serializers/StructDeserializer.hpp>
#include <srs/utils/ValidData.hpp>

namespace srs
{
    class Deserializers
    {
      public:
        Deserializers() = default;
        using enum DataDeserializeOptions;

        void deserialize(DataDeserializeOptions option);

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
            if (struct_data_.is_valid())
            {
                reset_struct_data(struct_data_.value());
                struct_data_.invalidate();
            }
            if (proto_data_.is_valid())
            {
                proto_data_->clear();
                proto_data_.invalidate();
            }
        }

      private:
        ValidData<SerializableMsgBuffer> binary_data_;
        ValidData<StructData> struct_data_;
        ValidData<std::string> proto_data_;
        StructDeserializer struct_deserializer_;
        ProtoDeserializer proto_deserializer_;

        void deserialize_struct()
        {
            if (struct_data_.is_valid())
            {
                return;
            }
            struct_deserializer_.convert(binary_data_->data(), struct_data_.value());
            struct_data_.validate();
        }

        void convert_to_proto()
        {
            if (proto_data_.is_valid())
            {
                return;
            }
            deserialize_struct();
            proto_deserializer_.convert(struct_data_.get(), proto_data_.value());
            proto_data_.validate();
        }
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
            return struct_data_.get();
        }
        else if constexpr (option == proto)
        {
            return proto_data_.get();
        }
        else
        {
            static_assert(false, "Cannot get the data from this option!");
        }
    };
} // namespace srs

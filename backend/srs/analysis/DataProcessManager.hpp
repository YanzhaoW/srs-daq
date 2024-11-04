#pragma once

#include "DataWriter.hpp"
#include <spdlog/spdlog.h>
#include <srs/Application.hpp>
#include <srs/analysis/DataStructs.hpp>
#include <srs/serializers/DataDeserializeOptions.hpp>
#include <srs/serializers/ProtoDeserializer.hpp>
#include <srs/serializers/SerializableBuffer.hpp>
#include <srs/serializers/StructDeserializer.hpp>
#include <srs/serializers/StructToProtoConverter.hpp>
#include <srs/utils/ValidData.hpp>

namespace srs
{
    class DataProcessManager
    {
      public:
        explicit DataProcessManager(DataProcessor* data_processor, asio::thread_pool& thread_pool);

        using enum DataDeserializeOptions;
        using StartingCoroType = asio::experimental::coro<std::string_view(bool)>;

        void analysis_one(auto& data_queue);

        // Getters:
        template <DataDeserializeOptions option>
        auto get_data() -> const auto&;

        void reset()
        {
            writers_.reset();
            binary_data_.clear();
        }

      private:
        SerializableMsgBuffer binary_data_;
        StructDeserializer struct_deserializer_;
        Struct2ProtoConverter struct_proto_converter_;
        ProtoDeserializer proto_deserializer_;

        StartingCoroType coro_;

        DataWriter writers_;

        auto generate_starting_coro(asio::any_io_executor /*unused*/) -> StartingCoroType;
    };

    template <DataDeserializeOptions option>
    auto DataProcessManager::get_data() -> const auto&
    {
        if constexpr (option == raw)
        {
            return binary_data_.data();
        }
        else if constexpr (option == structure)
        {
            return struct_deserializer_.data();
        }
        else if constexpr (option == proto)
        {
            return std::string_view{ proto_deserializer_.data() };
        }
        else
        {
            static_assert(false, "Cannot get the data from this option!");
        }
    };
} // namespace srs

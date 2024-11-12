#pragma once

#include <spdlog/spdlog.h>
#include <srs/Application.hpp>
#include <srs/converters/DataDeserializeOptions.hpp>
#include <srs/converters/ProtoDelimSerializer.hpp>
#include <srs/converters/ProtoSerializer.hpp>
#include <srs/converters/SerializableBuffer.hpp>
#include <srs/converters/StructDeserializer.hpp>
#include <srs/converters/StructToProtoConverter.hpp>
#include <srs/data/DataStructs.hpp>
#include <srs/utils/ValidData.hpp>
#include <srs/writers/DataWriter.hpp>
#include <tbb/concurrent_queue.h>

namespace srs
{
    class DataProcessManager
    {
      public:
        explicit DataProcessManager(DataProcessor* data_processor, asio::thread_pool& thread_pool);

        using enum DataDeserializeOptions;
        using StartingCoroType = asio::experimental::coro<std::string_view(bool)>;

        void analysis_one(tbb::concurrent_bounded_queue<SerializableMsgBuffer>& data_queue, bool is_stopped = false);
        void set_output_filenames(const std::vector<std::string>& filenames);
        void reset();
        void stop() { run_processes(true); }

        // Getters:
        template <DataDeserializeOptions option>
        auto get_data() -> const auto&;

      private:
        SerializableMsgBuffer binary_data_;
        StructDeserializer struct_deserializer_;
        Struct2ProtoConverter struct_proto_converter_;
        ProtoSerializer proto_serializer_;
        ProtoDelimSerializer proto_delim_serializer_;

        StartingCoroType coro_;

        DataWriter writers_;

        auto generate_starting_coro(asio::any_io_executor /*unused*/) -> StartingCoroType;
        void run_processes(bool is_stopped);
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
            return std::string_view{ proto_serializer_.data() };
        }
        else
        {
            static_assert(false, "Cannot get the data from this option!");
        }
    };
} // namespace srs

#pragma once

#include <expected>
#include <spdlog/spdlog.h>
#include <srs/Application.hpp>
#include <srs/converters/DataConvertOptions.hpp>
#include <srs/converters/ProtoDelimSerializer.hpp>
#include <srs/converters/ProtoSerializer.hpp>
#include <srs/converters/RawToDelimRawConveter.hpp>
#include <srs/converters/SerializableBuffer.hpp>
#include <srs/converters/StructDeserializer.hpp>
#include <srs/converters/StructToProtoConverter.hpp>
#include <srs/data/SRSDataStructs.hpp>
#include <srs/utils/ValidData.hpp>
#include <srs/writers/DataWriter.hpp>
#include <tbb/concurrent_queue.h>

namespace srs
{
    class DataProcessManager
    {
      public:
        explicit DataProcessManager(DataProcessor* data_processor, asio::thread_pool& thread_pool);

        // rule of 5
        ~DataProcessManager();
        DataProcessManager(const DataProcessManager&) = delete;
        DataProcessManager(DataProcessManager&&) = delete;
        DataProcessManager& operator=(const DataProcessManager&) = delete;
        DataProcessManager& operator=(DataProcessManager&&) = delete;

        using enum DataConvertOptions;
        using StartingCoroType = asio::experimental::coro<std::string_view(bool)>;

        auto analysis_one(tbb::concurrent_bounded_queue<SerializableMsgBuffer>& data_queue, bool is_blocking) -> bool;
        void set_output_filenames(const std::vector<std::string>& filenames);
        void reset();
        // void stop() { run_processes(true); }

        // Getters:
        template <DataConvertOptions option>
        auto get_data() -> std::string_view;

        auto get_struct_data() -> const auto& { return struct_deserializer_.data(); }

      private:
        SerializableMsgBuffer binary_data_;
        Raw2DelimRawConverter raw_to_delim_raw_converter_;
        StructDeserializer struct_deserializer_;
        Struct2ProtoConverter struct_proto_converter_;
        ProtoSerializer proto_serializer_;
        ProtoDelimSerializer proto_delim_serializer_;

        StartingCoroType coro_;

        writer::Manager writers_;

        auto generate_starting_coro(asio::any_io_executor /*unused*/) -> StartingCoroType;
        [[maybe_unused]] auto run_processes(bool is_stopped) -> std::expected<void, std::string_view>;
    };

    template <DataConvertOptions option>
    auto DataProcessManager::get_data() -> std::string_view
    {
        if constexpr (option == raw)
        {
            return binary_data_.data();
        }
        else if constexpr (option == proto)
        {
            return proto_serializer_.data();
        }
        else
        {
            static_assert(false, "Cannot get the data from this option!");
        }
    };
} // namespace srs

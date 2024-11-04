#pragma once

#include "DataStructs.hpp"
#include <fstream>
#include <glaze/glaze.hpp>
#include <map>
#include <spdlog/spdlog.h>
#include <srs/analysis/DataProcessManager.hpp>

namespace srs
{
    struct CompactExportData
    {
        ReceiveDataHeader header{};
        std::size_t marker_size{};
        std::size_t hit_size{};
        std ::map<std::string, std::vector<uint64_t>> marker_data{ { "vmm_id", {} }, { "srs_timestamp", {} } };
        std ::map<std::string, std::vector<uint16_t>> hit_data{ { "is_over_threshold", {} },
                                                                { "channel_num", {} },
                                                                { "tdc", {} },
                                                                { "offset", {} },
                                                                { "vmm_id", {} },
                                                                { "adc", {} },
                                                                { "bc_id", {} } };
        // HitDataVec hit_data;

        void set_value(const StructData& data_struct)
        {
            header = data_struct.header;
            fill_hit_data(data_struct.hit_data);
            fill_marker_data(data_struct.marker_data);
        }

      private:
        void fill_hit_data(const std::vector<HitData>& hits)
        {
            hit_size = hits.size();
            for (auto& [key, value] : hit_data)
            {
                value.clear();
                value.reserve(hit_size);
            }
            for (const auto& hit : hits)
            {
                hit_data.at("is_over_threshold").push_back(static_cast<uint16_t>(hit.is_over_threshold));
                hit_data.at("channel_num").push_back(hit.channel_num);
                hit_data.at("tdc").push_back(hit.tdc);
                hit_data.at("offset").push_back(hit.offset);
                hit_data.at("vmm_id").push_back(hit.vmm_id);
                hit_data.at("adc").push_back(hit.adc);
                hit_data.at("bc_id").push_back(hit.bc_id);
            }
        }

        void fill_marker_data(const std::vector<MarkerData>& markers)
        {
            marker_size = markers.size();
            for (auto& [key, value] : marker_data)
            {
                value.clear();
                value.reserve(marker_size);
            }
            for (const auto& marker : markers)
            {
                marker_data.at("vmm_id").push_back(marker.vmm_id);
                marker_data.at("srs_timestamp").push_back(marker.srs_timestamp);
            }
        }
    };

    class JsonWriter
    {
      public:
        explicit JsonWriter(std::string_view filename)
            : file_stream_{ filename.data(), std::ios::out | std::ios::trunc }
        {
            if (not file_stream_.is_open())
            {
                spdlog::critical("JsonWriter: cannot open the file with filename {:?}", filename);
                throw std::runtime_error("Error occured with JsonWriter");
            }
            file_stream_ << "[\n";
        }
        static constexpr auto IsStructType = true;
        [[nodiscard]] static auto get_deserialize_mode() -> DataDeserializeOptions
        {
            return DataDeserializeOptions::structure;
        }
        auto write(auto fut) -> boost::unique_future<std::optional<int>> { return {}; }

        ~JsonWriter()
        {
            file_stream_ << "]\n";
            file_stream_.close();
        }

        void write(const StructData& data_struct)
        {
            data_buffer_.set_value(data_struct);
            auto error_code =
                glz::write<glz::opts{ .prettify = 1, .new_lines_in_arrays = 0 }>(data_buffer_, string_buffer_);
            if (error_code)
            {
                spdlog::critical("JsonWriter: cannot interpret data struct to json. Error: {}",
                                 error_code.custom_error_message);
                throw std::runtime_error("Error occured with JsonWriter");
            }
            file_stream_ << string_buffer_;
            string_buffer_.clear();
        }

        void write(std::string_view str) { file_stream_ << str; }

      private:
        std::fstream file_stream_;
        CompactExportData data_buffer_;
        std::string string_buffer_;

      public:
        JsonWriter(const JsonWriter&) = delete;
        JsonWriter(JsonWriter&&) = delete;
        JsonWriter& operator=(const JsonWriter&) = delete;
        JsonWriter& operator=(JsonWriter&&) = delete;
    };

} // namespace srs

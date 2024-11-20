#pragma once

#include <boost/thread/future.hpp>
#include <map>
#include <spdlog/spdlog.h>
#include <srs/converters/DataConvertOptions.hpp>
#include <srs/data/SRSDataStructs.hpp>
#include <srs/writers/DataWriterOptions.hpp>

namespace srs
{
    class RootFileWriter;
    class DataProcessor;
    class BinaryFileWriter;
    class UDPWriter;
    class JsonWriter;
    class DataProcessManager;

    class DataWriter
    {
      public:
        using enum DataWriterOption;
        using enum DataConvertOptions;

        explicit DataWriter(DataProcessor* processor);

        ~DataWriter();
        DataWriter(const DataWriter&) = delete;
        DataWriter& operator=(const DataWriter&) = delete;
        DataWriter(DataWriter&&) = default;
        DataWriter& operator=(DataWriter&&) = default;

        void write_with(auto make_future);
        void wait_for_finished();
        void reset() { write_futures_.clear(); }
        void set_output_filenames(const std::vector<std::string>& filenames);

        // Getter:
        [[nodiscard]] auto is_convert_required(DataConvertOptions dependee) const -> bool;

      private:
        std::map<DataConvertOptions, int> convert_count_map_{ EMPTY_CONVERT_OPTION_COUNT_MAP.begin(),
                                                              EMPTY_CONVERT_OPTION_COUNT_MAP.end() };
        std::map<std::string, std::unique_ptr<BinaryFileWriter>> binary_files_;
        std::map<std::string, std::unique_ptr<UDPWriter>> udp_files_;
        std::map<std::string, std::unique_ptr<JsonWriter>> json_files_;
#ifdef HAS_ROOT
        std::map<std::string, std::unique_ptr<RootFileWriter>> root_files_;
#endif
        DataProcessor* data_processor_ = nullptr;
        std::vector<boost::unique_future<std::optional<int>>> write_futures_;

        auto add_binary_file(const std::string& filename, DataConvertOptions deser_mode) -> bool;
        auto add_udp_file(const std::string& filename, DataConvertOptions deser_mode) -> bool;
        auto add_root_file(const std::string& filename) -> bool;
        auto add_json_file(const std::string& filename) -> bool;

        template <typename WriterType>
        void write_to_files(std::map<std::string, std::unique_ptr<WriterType>>& writers, auto make_future)
        {
            for (auto& [key, writer] : writers)
            {
                auto fut = make_future(*writer);
                write_futures_.emplace_back(std::move(fut));
            }
        }
    };

    void DataWriter::write_with(auto make_future)
    {
        write_to_files(binary_files_, make_future);
        write_to_files(udp_files_, make_future);
        write_to_files(json_files_, make_future);
#ifdef HAS_ROOT
        write_to_files(root_files_, make_future);
#endif
    }
} // namespace srs

#pragma once

#include <algorithm>
#include <fstream>
#include <srs/converters/RawToDelimRawConveter.hpp>
#include <srs/utils/CommonDefitions.hpp>
#include <string>
#include <vector>
#include <print>

namespace srs
{
    class RawFrameReader
    {
      public:
        explicit RawFrameReader(const std::string& filename)
            : input_filename_{ filename }
            , input_file_{ filename, std::ios::binary }
        {
            if (input_file_.fail())
            {
                throw std::runtime_error{ fmt::format("Cannot open the file {:?}", input_filename_) };
            }
            spdlog::debug("Open the binary file {:?}", input_filename_);
        }

        void read_one_frame(std::vector<char>& data_str)
        {
            auto data_view = read_one_frame();
            data_str.clear();
            data_str.reserve(data_view.size());
            std::ranges::copy(data_view, std::back_inserter(data_str));
        }

        auto read_one_frame() -> std::string_view
        {
            std::ranges::fill(input_buffer_, 0);
            auto size = Raw2DelimRawConverter::SizeType{};

            if (input_file_.eof())
            {
                return {};
            }

            std::println("==================passing here 3");
            input_file_ >> size;
            auto read_size = static_cast<std::size_t>(input_file_.read(input_buffer_.data(), size).gcount());

            if (read_size != size)
            {
                spdlog::warn("Binary Data is not extracted correctly from the file {:?}", input_filename_);
            }

            return std::string_view{ input_buffer_.data(), read_size };
        }

      private:
        std::string input_filename_;
        std::ifstream input_file_;
        std::array<char, LARGE_READ_MSG_BUFFER_SIZE> input_buffer_{};
    };
} // namespace srs

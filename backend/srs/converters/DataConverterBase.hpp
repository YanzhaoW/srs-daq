#pragma once

#include <algorithm>
#include <boost/asio/experimental/coro.hpp>
#include <srs/converters/DataDeserializeOptions.hpp>
#include <srs/utils/CommonFunctions.hpp>
#include <srs/writers/DataWriter.hpp>

namespace srs
{
    template <typename Input, typename Output>
    class DataConverterBase
    {
      public:
        using InputType = Input;
        using OutputType = Output;
        using InputFuture = boost::shared_future<std::optional<InputType>>;
        using OutputFuture = boost::shared_future<std::optional<OutputType>>;
        using CoroType = asio::experimental::coro<OutputType(std::optional<InputType>)>;

        using enum DataDeserializeOptions;

        explicit DataConverterBase(auto coro)
            : coro_(std::move(coro))
        {

            coro_sync_start(coro_, std::optional<InputType>{}, asio::use_awaitable);
        }

        auto create_future(this auto&& self, InputFuture& pre_fut, DataWriter& writers) -> OutputFuture
        {
            constexpr auto converter_options = std::remove_cvref_t<decltype(self)>::ConverterOption;
            auto is_needed = std::ranges::any_of(
                converter_options, [&writers](auto option) { return writers.has_deserialize_type(option); });
            return is_needed ? create_coro_future(self.coro_, pre_fut) : OutputFuture{};
        }

      private:
        CoroType coro_;
    };
} // namespace srs
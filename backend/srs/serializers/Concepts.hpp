#pragma once

#include <fstream>

namespace srs
{
    template <typename T>
    concept Deserializer = requires(T) {
        typename T::OutputType;
        typename T::InputType;
    };

    template <typename T>
    concept StreamableOutput = requires(T obj) { std::declval<std::ofstream>() << obj; };

    template <typename T>
    concept StreamableDeserializer = Deserializer<T> and StreamableOutput<typename T::OutputType>;
}; // namespace srs

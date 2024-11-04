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

    template <typename T>
    concept RangedData = requires(T obj) {
        std::contiguous_iterator<decltype(obj.begin())>;
        std::contiguous_iterator<decltype(obj.end())>;
    };

    template <typename DataType, typename TaskType>
    concept OutputCompatible = requires(DataType, TaskType task) {
        std::is_same_v<DataType, std::remove_cvref_t<decltype(task.get_output_data())>>;
    };

}; // namespace srs

#pragma once

#include <concepts>
#include <stdexcept>

namespace srs
{

    template <std::default_initializable T>
    class ValidData
    {
      public:
        ValidData() = default;

        auto value() -> auto&
        {
            if (not is_valid_)
            {
                throw std::runtime_error("Cannot call value(): ValidData value is not valid!");
            }
            return resource_;
        }

        auto get() const -> const auto&
        {
            if (not is_valid_)
            {
                throw std::runtime_error("Cannot call get(): ValidData value is not valid!");
            }
            return resource_;
        }

        auto is_valid() -> bool { return is_valid_; }

        void invalidate() { is_valid_ = false; }
        void validate() { is_valid_ = true; }

        auto operator->() -> T* { return &resource_; }

      private:
        bool is_valid_ = false;
        T resource_;
    };

} // namespace srs

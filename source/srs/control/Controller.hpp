#pragma once

namespace srs
{

    class Controller
    {

      public:
        Controller() = default;
        void switch_on();
        void switch_off();
        void read_data();

      private:
    };

} // namespace srs

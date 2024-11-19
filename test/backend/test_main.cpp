#include <gtest/gtest.h>
#include <srs/SRSEmulator.hpp>

namespace srs::test
{
    namespace
    {
        void run_application()
        {

            try
            {
                auto app = srs::App{};
                auto emulator = SRSEmulator{ "test_data.bin", app };

                const auto& remote_endpoint = emulator.get_remote_endpoint();
                app.set_fec_data_receiv_port(remote_endpoint.port());

                app.read_data();
                app.start_analysis();

                emulator.start_send_data();
                app.exit();
            }
            catch (const std::runtime_error& err)
            {
                spdlog::critical("Exception occurred: {}", err.what());
            }
        }

    } // namespace

} // namespace srs::test

auto main(int argc, char** argv) -> int
{
    spdlog::set_level(spdlog::level::trace);
    ::testing::InitGoogleTest(&argc, argv);
    srs::test::run_application();
    return RUN_ALL_TESTS();
}

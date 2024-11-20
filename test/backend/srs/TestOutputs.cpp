#include <filesystem>
#include <gtest/gtest.h>
#include <srs/SRSEmulator.hpp>
#include <srs/utils/Connections.hpp>

namespace srs::test
{
    namespace
    {
        void run_application(const std::string& output_filename)
        {
            try
            {
                auto app = srs::App{};
                auto output_filenames = std::vector<std::string>{};
                output_filenames.push_back(output_filename);

                app.set_fec_data_receiv_port(0);
                app.set_output_filenames(output_filenames);

                app.read_data();
                auto* data_reader = app.get_data_reader();
                auto port_num = data_reader->get_local_port_number();
                auto emulator = SRSEmulator{ "test_data.bin", port_num, app };

                app.start_analysis();

                emulator.start_send_data();
                app.exit();
                app.wait_for_finish();
            }
            catch (const std::runtime_error& err)
            {
                spdlog::critical("Exception occurred: {}", err.what());
            }
        }

    } // namespace

} // namespace srs::test

namespace
{

    namespace fs = std::filesystem;

    auto check_if_file_exist(const std::string& filename) -> bool
    {
        auto file_path = fs::path{ filename };
        return fs::exists(file_path);
    }

} // namespace

TEST(check_outputs, binary_output)
{
    const auto filename = std::string{ "test_output.bin" };
    srs::test::run_application(filename);
    auto res = check_if_file_exist(filename);
    ASSERT_TRUE(res);
}

TEST(check_outputs, root_output)
{
    const auto filename = std::string{ "test_output.root" };
    srs::test::run_application(filename);
    auto res = check_if_file_exist(filename);
#ifdef HAS_ROOT
    ASSERT_TRUE(res);
#else
    ASSERT_FALSE(res);
#endif
}

TEST(check_outputs, proto_binary)
{
    const auto filename = std::string{ "test_output.binpb" };
    srs::test::run_application(filename);
    auto res = check_if_file_exist(filename);
    ASSERT_TRUE(res);
}

TEST(check_outputs, json_output)
{
    const auto filename = std::string{ "test_output.json" };
    srs::test::run_application(filename);
    auto res = check_if_file_exist(filename);
#ifdef HAS_ROOT
    ASSERT_TRUE(res);
#else
    ASSERT_FALSE(res);
#endif
}

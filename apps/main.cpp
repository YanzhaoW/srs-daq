#include "CLIOptionsMap.hpp"
#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include <srs/Application.hpp>

auto main(int argc, char** argv) -> int
{
    auto cli_args = CLI::App{ "SRS system command line interface" };
    try
    {
        argv = cli_args.ensure_utf8(argv);

        auto spdlog_level = spdlog::level::info;
        auto print_mode = srs::DataPrintMode::print_speed;

        cli_args.add_option("-v, --verbose-level", spdlog_level, "set log level")
            ->transform(CLI::CheckedTransformer(spd_log_map, CLI::ignore_case))
            ->capture_default_str();
        cli_args.add_option("-p, --print-mode", print_mode, "set print mode")
            ->transform(CLI::CheckedTransformer(print_mode_map, CLI::ignore_case))
            ->capture_default_str();
        cli_args.parse(argc, argv);

        spdlog::set_level(spdlog_level);

        auto app = srs::App{};
        app.set_remote_endpoint("10.0.0.2", 6600);
        app.set_print_mode(print_mode);
        app.read_data();
        app.switch_on();
        app.run();
    }
    catch (const CLI::ParseError& e)
    {
        cli_args.exit(e);
    }
    catch (std::exception& ex)
    {
        spdlog::critical("Exception occured: {}", ex.what());
    }

    return 0;
}

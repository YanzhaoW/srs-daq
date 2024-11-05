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
        auto output_filenames = std::vector<std::string>{ "output.bin" };

        cli_args.add_option("-v, --verbose-level", spdlog_level, "set log level")
            ->transform(CLI::CheckedTransformer(spd_log_map, CLI::ignore_case))
            ->capture_default_str();
        cli_args.add_option("-p, --print-mode", print_mode, "set data print mode")
            ->transform(CLI::CheckedTransformer(print_mode_map, CLI::ignore_case))
            ->capture_default_str();
        cli_args.add_option("-o, --output-files", output_filenames, "set output file (or socket) names")
            ->capture_default_str()
            ->expected(0, -1);
        cli_args.parse(argc, argv);

        spdlog::set_level(spdlog_level);

        auto app = srs::App{};
        app.set_remote_endpoint(srs::DEFAULT_SRS_IP, srs::DEFAULT_SRS_CONTROL_PORT);
        app.set_print_mode(print_mode);
        app.set_output_filenames(output_filenames);


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

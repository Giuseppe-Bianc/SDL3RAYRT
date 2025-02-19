// NOLINTBEGIN(*-include-cleaner)
#include <CLI/CLI.hpp>
#include <sdl3rayrt_lib/sdl3rayrt.hpp>

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv) {
    INIT_LOG()
    try {
        CLI::App app{GENERATOR_FULLNAME};

        /*std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        */
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        /*bool is_turn_based = false;
        auto *turn_based = app.add_flag("--turn_based", is_turn_based);

        bool is_loop_based = false;
        auto *loop_based = app.add_flag("--loop_based", is_loop_based);

        turn_based->excludes(loop_based);
        loop_based->excludes(turn_based);*/

        CLI11_PARSE(app, argc, argv);
        /*if (show_version) {
          fmt::print("{}\n", SDL3RAYRT::cmake::project_version);
          return EXIT_SUCCESS;
        }*/
        sdlrt::CApp theapp;
        return theapp.OnExecute();
    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
}
// NOLINTEND(*-include-cleaner)

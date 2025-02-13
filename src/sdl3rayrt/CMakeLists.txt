add_executable(sdl3rayrt main.cpp)

target_link_libraries(
        sdl3rayrt
        PRIVATE SDL3RAYRT::SDL3RAYRT_options
        SDL3RAYRT::SDL3RAYRT_warnings)

target_link_system_libraries(
        sdl3rayrt
        PRIVATE
        CLI11::CLI11
        SDL3RAYRT::sdl3rayrt_lib
)

target_include_directories(sdl3rayrt PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
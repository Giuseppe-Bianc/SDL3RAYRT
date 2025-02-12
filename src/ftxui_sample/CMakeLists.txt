add_executable(intro main.cpp)

target_link_libraries(
  intro
  PRIVATE SDL3RAYRT::SDL3RAYRT_options
          SDL3RAYRT::SDL3RAYRT_warnings)

target_link_system_libraries(
  intro
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog
          SDL3::SDL3
)

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
#[[
if(MSVC)
    target_compile_options(intro PRIVATE /analyze  /analyze:external-)
endif()
]]


include(GenerateExportHeader)

#find_package(glm REQUIRED)
add_library(sdl3rayrt_lib sdl3rayrt.cpp
        CApp.cpp
        ../../include/sdl3rayrt_lib/CApp.hpp
        FPSCounter.cpp
        ../../include/sdl3rayrt_lib/FPSCounter.hpp
        rayTrace/Image.cpp
        ../../include/sdl3rayrt_lib/rayTrace/Image.hpp
        rayTrace/Ray.cpp
        ../../include/sdl3rayrt_lib/rayTrace/Ray.hpp
        rayTrace/Scene.cpp
        ../../include/sdl3rayrt_lib/rayTrace/Scene.hpp
        rayTrace/Camera.cpp
        ../../include/sdl3rayrt_lib/rayTrace/Camera.hpp
        rayTrace/ObjectBase.cpp
        ../../include/sdl3rayrt_lib/rayTrace/ObjectBase.hpp
        rayTrace/ObjSphere.cpp
        ../../include/sdl3rayrt_lib/rayTrace/ObjSphere.hpp
        rayTrace/LightBase.cpp
        ../../include/sdl3rayrt_lib/rayTrace/LightBase.hpp
        rayTrace/PointLight.cpp
        ../../include/sdl3rayrt_lib/rayTrace/PointLight.hpp
        rayTrace/GTform.cpp
        ../../include/sdl3rayrt_lib/rayTrace/GTform.hpp)

add_library(SDL3RAYRT::sdl3rayrt_lib ALIAS sdl3rayrt_lib)

# Retrieve the target name
#get_target_property(target_name vandior_core_lib NAME)
#include("${CMAKE_SOURCE_DIR}/cmake/Simd.cmake")
#set_simd_instructions(${target_name})


target_link_libraries(sdl3rayrt_lib
        PRIVATE
        SDL3RAYRT_options
        SDL3RAYRT_warnings
        PUBLIC
        SDL3RAYRT::sdl3rayrt_core_lib
)

target_include_directories(sdl3rayrt_lib
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        "${CMAKE_BINARY_DIR}/configured_files/include"
)
if (SDL3RAYRT_BUILD_FUZZ_TESTS)
    target_link_libraries(sdl3rayrt_lib PRIVATE -fsanitize=fuzzer-no-link)
    target_compile_options(sdl3rayrt_lib PRIVATE -fsanitize=fuzzer-no-link)
endif ()

target_compile_features(sdl3rayrt_lib PUBLIC cxx_std_${CMAKE_CXX_STANDARD})


set_target_properties(
        sdl3rayrt_lib
        PROPERTIES VERSION ${PROJECT_VERSION}
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(sdl3rayrt_lib EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/SDL3RAYRT/sdl3rayrt_lib_export.hpp)

if (NOT BUILD_SHARED_LIBS)
    target_compile_definitions(sdl3rayrt_lib PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif ()

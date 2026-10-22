cmake_minimum_required(VERSION 3.10)

project(RayTracer LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# The scene programs use M_PI, M_PI_2, and M_PI_4.
set(CMAKE_C_EXTENSIONS ON)

add_library(raytracer_core STATIC
    src/camera.c
    src/canvas.c
    src/intersection.c
    src/light.c
    src/material.c
    src/matrix.c
    src/ray.c
    src/sphere.c
    src/transform.c
    src/tuple.c
    src/world.c
)

target_include_directories(raytracer_core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# light_create() and material_create() currently have non-static inline
# definitions in .c files. Preserve externally linkable definitions.
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(raytracer_core PRIVATE -fgnu89-inline)
endif()

if(UNIX)
    target_link_libraries(raytracer_core PUBLIC m)
endif()

add_executable(raytracer main.c)
target_link_libraries(raytracer PRIVATE raytracer_core)

add_executable(raytracer_chapter7 main_7.c)
target_link_libraries(raytracer_chapter7 PRIVATE raytracer_core)

# Chapter 8 renders the same scene with the new shadow-aware world shading.
add_executable(raytracer_chapter8 main_7.c)
target_compile_definitions(raytracer_chapter8 PRIVATE
    RAYTRACER_OUTPUT_FILE="./chapter8.bmp"
)
target_link_libraries(raytracer_chapter8 PRIVATE raytracer_core)

# These existing programs are smoke tests; most print results rather than
# asserting expected values. Disable them with -DBUILD_TESTING=OFF.
include(CTest)
if(BUILD_TESTING)
    foreach(test_name camera intersection transform world)
        add_executable(${test_name}_test tests/${test_name}_test.c)
        target_link_libraries(${test_name}_test PRIVATE raytracer_core)
        add_test(NAME ${test_name} COMMAND ${test_name}_test)
    endforeach()

    # Assertions in this suite remain active in Release builds.
    add_executable(shadows_test tests/shadows_test.c)
    target_link_libraries(shadows_test PRIVATE raytracer_core)
    add_test(NAME shadows COMMAND shadows_test)
endif()

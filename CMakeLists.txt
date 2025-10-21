cmake_minimum_required(VERSION 4.1)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)


# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/raytracer.cpp
    ${PROJECT_SOURCE_DIR}/src/material.cpp
    ${PROJECT_SOURCE_DIR}/src/color.cpp
    ${PROJECT_SOURCE_DIR}/src/math/ray.cpp
    ${PROJECT_SOURCE_DIR}/src/geometry/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/geometry/plane.cpp
    ${PROJECT_SOURCE_DIR}/src/primitive.cpp
    ${PROJECT_SOURCE_DIR}/src/math/vector3.cpp
    ${PROJECT_SOURCE_DIR}/src/math/floatutil.cpp
    ${PROJECT_SOURCE_DIR}/src/math/point3.cpp
    ${PROJECT_SOURCE_DIR}/src/math/point2.cpp
    ${PROJECT_SOURCE_DIR}/src/light/lightp.cpp
    ${PROJECT_SOURCE_DIR}/src/light/lightd.cpp
    ${PROJECT_SOURCE_DIR}/src/scene.cpp
    ${PROJECT_SOURCE_DIR}/src/texture.cpp
    ${PROJECT_SOURCE_DIR}/src/camera.cpp
    
)

set(
    INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src
)

include_directories(${INCLUDE_DIRS})
add_executable(raytracer ${SOURCES})

target_link_libraries(raytracer nlohmann_json::nlohmann_json)

# Add compiler optimizations
#if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#   target_compile_options(raytracer PRIVATE -O3)
#elseif(MSVC)
#  target_compile_options(raytracer PRIVATE /O2)
#endif()

# Add debugging flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

add_custom_command(
    TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/files"
        "$<TARGET_FILE_DIR:raytracer>"
)


enable_testing()

add_executable(
    raytracer_tests
    ${PROJECT_SOURCE_DIR}/src/math/vector3.cpp
    ${PROJECT_SOURCE_DIR}/src/math/point3.cpp
    ${PROJECT_SOURCE_DIR}/src/math/point2.cpp
    ${PROJECT_SOURCE_DIR}/src/math/floatutil.cpp
    ${PROJECT_SOURCE_DIR}/tests/vector3_test.cpp
    ${PROJECT_SOURCE_DIR}/tests/point3_test.cpp
    ${PROJECT_SOURCE_DIR}/tests/point2_test.cpp
)

target_link_libraries(
  raytracer_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(raytracer_tests)
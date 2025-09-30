cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hw1a)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/raytracer.cpp
    ${PROJECT_SOURCE_DIR}/src/material.cpp
    ${PROJECT_SOURCE_DIR}/src/color.cpp
    ${PROJECT_SOURCE_DIR}/src/math/ray.cpp
    ${PROJECT_SOURCE_DIR}/src/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/math/vector3.cpp
    ${PROJECT_SOURCE_DIR}/src/math/floatutil.cpp
    ${PROJECT_SOURCE_DIR}/src/math/point3.cpp
    ${PROJECT_SOURCE_DIR}/src/light.cpp
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

# Add compiler optimizations
#if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
#    target_compile_options(raytracer1c PRIVATE -O3)
#elseif(MSVC)
#   target_compile_options(raytracer1c PRIVATE /O2)
#endif()

# Add debugging flags
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

add_custom_command(
    TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/files"
        "$<TARGET_FILE_DIR:raytracer>"
)



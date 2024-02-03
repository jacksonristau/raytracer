cmake_minimum_required(VERSION 3.1)

project(hw1a)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/raytracer1a.cpp
    ${PROJECT_SOURCE_DIR}/math/point.cpp
    ${PROJECT_SOURCE_DIR}/math/ray.cpp
    ${PROJECT_SOURCE_DIR}/math/sphere.cpp
    ${PROJECT_SOURCE_DIR}/math/vector.cpp
)

set(
    INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/math/
)

include_directories(INCLUDE_DIRS)

add_executable(raytracer1a ${SOURCES})
target_compile_options(raytracer1a PRIVATE -g -Wno-narrowing)
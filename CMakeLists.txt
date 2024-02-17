cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(hw1a)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/raytracer1a.cpp
    ${PROJECT_SOURCE_DIR}/src/material.cpp
    ${PROJECT_SOURCE_DIR}/src/color.cpp
    ${PROJECT_SOURCE_DIR}/src/ray.cpp
    ${PROJECT_SOURCE_DIR}/src/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/vector.cpp
    ${PROJECT_SOURCE_DIR}/src/light.cpp
)

set(
    INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src
)

include_directories(INCLUDE_DIRS)

add_executable(raytracer1a ${SOURCES})
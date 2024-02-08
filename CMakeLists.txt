cmake_minimum_required(VERSION 3.1)

project(hw1a)

set(
    SOURCES
    ${PROJECT_SOURCE_DIR}/src/raytracer1a.cpp
    ${PROJECT_SOURCE_DIR}/src/material.cpp
    ${PROJECT_SOURCE_DIR}/src/color.cpp
    ${PROJECT_SOURCE_DIR}/src/point.cpp
    ${PROJECT_SOURCE_DIR}/src/ray.cpp
    ${PROJECT_SOURCE_DIR}/src/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/vector.cpp
)

set(
    INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src
)

include_directories(INCLUDE_DIRS)

add_executable(raytracer1a ${SOURCES})
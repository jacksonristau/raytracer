cmake_minimum_required(VERSION 3.1)

project(hw1a)

set(
    SOURCES
    raytracer1a.cpp
)

set(
    INCLUDE_DIRS
    math/
)

add_executable(raytracer1a ${SOURCES})
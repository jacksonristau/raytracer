cmake_minimum_required(VERSION 3.15...4.0)

project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


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
    ${PROJECT_SOURCE_DIR}/include
)

include_directories(${INCLUDE_DIRS})
add_executable(raytracer ${SOURCES})

target_link_libraries(raytracer nlohmann_json::nlohmann_json)

add_custom_command(
    TARGET raytracer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/files"
        "$<TARGET_FILE_DIR:raytracer>"
)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MYPROJECT_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
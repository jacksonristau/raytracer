file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

set(
  SOURCE_LIST
  ${PROJECT_SOURCE_DIR}/src/geometry/plane.cpp
  ${PROJECT_SOURCE_DIR}/src/geometry/primitive.cpp
  ${PROJECT_SOURCE_DIR}/src/geometry/triangle.cpp
  ${PROJECT_SOURCE_DIR}/src/geometry/sphere.cpp
  ${PROJECT_SOURCE_DIR}/src/geometry/triangle.cpp
  ${PROJECT_SOURCE_DIR}/src/geometry/hit.cpp
  
  ${PROJECT_SOURCE_DIR}/src/gfx/camera.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/color.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/lights.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/material.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/scene.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/texture.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/raytracer.cpp
  ${PROJECT_SOURCE_DIR}/src/gfx/tiny_obj_loader.cpp


  ${PROJECT_SOURCE_DIR}/src/math/floatutil.cpp
  ${PROJECT_SOURCE_DIR}/src/math/point2.cpp
  ${PROJECT_SOURCE_DIR}/src/math/point3.cpp
  ${PROJECT_SOURCE_DIR}/src/math/ray.cpp
  ${PROJECT_SOURCE_DIR}/src/math/vector3.cpp

  ${PROJECT_SOURCE_DIR}/src/stringhelpers.cpp
)

add_library(raytracerlib ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(raytracerlib PUBLIC ../include)

target_link_libraries(raytracerlib PUBLIC nlohmann_json::nlohmann_json)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "headers"
  FILES ${HEADER_LIST})

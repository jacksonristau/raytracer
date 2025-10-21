file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(raytracerlib ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(raytracerlib PUBLIC ../include)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "headers"
  FILES ${HEADER_LIST})

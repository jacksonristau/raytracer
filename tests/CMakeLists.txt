FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

# set(
#     TEST_LIST
#     floatutils_test.cpp
#     vector3_test.cpp
#     point3_test.cpp
#     scene_test.cpp
#     aabb_test.cpp
# )

function(add_test_executable name)
    add_executable(${name} ${ARGN})
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    if(MSVC)
        set_target_properties(${name} PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            VS_USER_PROPS "${CMAKE_CURRENT_SOURCE_DIR}/my-app.vcxproj.user"
        )
    endif()
    target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain raytracerlib)
    catch_discover_tests(${name})
endfunction()

add_test_executable(vector_tests vector3_test.cpp point3_test.cpp)

add_test_executable(scene_tests scene_test.cpp)

add_test_executable(float_tests floatutils_test.cpp)

add_test_executable(aabb_tests aabb_test.cpp)
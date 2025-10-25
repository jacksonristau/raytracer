FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)

add_executable(raytests vector3_test.cpp)

# These help VS recognize it as a runnable target
set_target_properties(raytests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Explicitly mark as executable (sometimes helps)
if(MSVC)
    set_target_properties(raytests PROPERTIES
        VS_USER_PROPS "${CMAKE_CURRENT_SOURCE_DIR}/my-app.vcxproj.user"
    )
endif()

catch_discover_tests(raytests)

# link to the main library, as well as Catch2
target_link_libraries(raytests PRIVATE Catch2::Catch2WithMain PRIVATE raytracerlib)
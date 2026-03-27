cmake_minimum_required(VERSION 3.15...4.0)

project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific Release optimization flags
if(MSVC)
  # /O2    - maximize speed (default for Release, explicit for clarity)
  # /Ob3   - aggressive inlining beyond /Ob2
  # /GL    - whole program optimization (pairs with /LTCG)
  # /fp:fast - relaxed IEEE float math for faster codegen
  # /Oi    - replace function calls with intrinsics
  # /Ot    - favor fast code over small code
  # /GS-   - disable buffer security checks
  # /Gy    - function-level linking (discard unused functions)
  # /Gw    - global data optimization
  # /arch:AVX2 - use AVX2 SIMD instructions
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob3 /GL /fp:fast /Oi /Ot /GS- /Gy /Gw /arch:AVX2")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /Ob3 /GL /fp:fast /Oi /Ot /GS- /Gy /Gw /arch:AVX2")
  # Link-time code generation (pairs with /GL)
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
  set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} /LTCG")
elseif(APPLE)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto -ffast-math -funroll-loops")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -flto -ffast-math -funroll-loops")
endif()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)
endif()

# Create symlink to resource files in build directory
file(CREATE_LINK
  ${CMAKE_SOURCE_DIR}/files
  ${CMAKE_BINARY_DIR}/files
  SYMBOLIC)

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

# add json library
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# raytracerlib code
add_subdirectory(src)

# executable code
add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
# how to use override: cmake -B build -S . -DMODERN_CMAKE_BUILD_TESTING=ON
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()
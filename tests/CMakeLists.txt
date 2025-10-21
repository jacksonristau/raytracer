FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)

add_executable(raytests hello_test.cpp)

# link to the main library, as well as Catch2
target_link_libraries(testlib PRIVATE modern_library Catch2::Catch2)

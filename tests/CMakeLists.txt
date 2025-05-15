find_package(Catch2 CONFIG REQUIRED)
if (NOT Catch2_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
  )
  FetchContent_MakeAvailable(catch2)

  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

add_executable(pbrt_tests)

set_target_properties(
  pbrt_tests
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_subdirectory("core")

target_sources(
  pbrt_tests
  PRIVATE
    "types.cpp"
)

include(CTest)
include(Catch)
catch_discover_tests(pbrt_tests)

target_link_libraries(
  pbrt_tests
  PRIVATE
    pbrt
    Catch2::Catch2WithMain
)

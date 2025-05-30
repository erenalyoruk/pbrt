find_package(Catch2 CONFIG QUIET)

if (NOT Catch2_FOUND)
  message(STATUS "Catch2 not found, fetching...")

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

target_sources(
  pbrt_tests
  PRIVATE
    "assertions.cpp"
    "types.cpp"
)

add_subdirectory("logging")
add_subdirectory("ecs")
add_subdirectory("math")
add_subdirectory("profiling")

include(CTest)
include(Catch)
catch_discover_tests(pbrt_tests)

target_link_libraries(
  pbrt_tests
  PRIVATE
    pbrt
    Catch2::Catch2WithMain
)

add_custom_command(
  TARGET pbrt_tests PRE_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy -t $<TARGET_FILE_DIR:pbrt_tests> $<TARGET_RUNTIME_DLLS:pbrt_tests>
  COMMAND_EXPAND_LISTS
)

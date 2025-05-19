cmake_minimum_required(VERSION 3.25)

project(pbrt LANGUAGES CXX VERSION 0.0.1)

option(PBRT_BUILD_TESTS "Build tests" ON)
option(PBRT_BUILD_TESTBED "Build testbed" ON)
option(PBRT_BUILD_DOCS "Build docs" ON)

add_library(pbrt_config INTERFACE)

target_compile_definitions(
  pbrt_config
  INTERFACE
    # build definitions
    $<$<CONFIG:Debug>:PBRT_BUILD_DEBUG>
    $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:PBRT_BUILD_RELEASE>

    # platform definitions
    $<$<PLATFORM_ID:Windows>:PBRT_PLATFORM_WINDOWS>

    # compiler definitions
    $<$<CXX_COMPILER_ID:MSVC>:PBRT_COMPILER_MSVC>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:PBRT_COMPILER_GCC>
)

target_compile_options(
  pbrt_config
  INTERFACE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive /Zc:__cplusplus /Zc:preprocessor>
)

add_library(pbrt)

set_target_properties(
  pbrt
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_definitions(
  pbrt
  PRIVATE
    PBRT_EXPORTS
)

target_include_directories(
  pbrt
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/src>
    $<INSTALL_INTERFACE:src>
)

target_link_libraries(
  pbrt
  PUBLIC
    pbrt_config
)

add_subdirectory("src")

if (PBRT_BUILD_TESTBED)
  add_subdirectory("testbed")
endif()

if (PBRT_BUILD_TESTS)
  enable_testing()
  add_subdirectory("tests")
endif()

if (PBRT_BUILD_DOCS)
  add_subdirectory("docs")
endif()

cmake_minimum_required(VERSION 3.25)

option(PBRT_BUILD_SHARED "Build shared library" ON)
option(PBRT_BUILD_TESTS "Build tests" ON)
option(PBRT_BUILD_TESTBED "Build testbed" ON)
option(PBRT_BUILD_DOCS "Build docs" ON)
option(PBRT_VERBOSE "Enable tracing" ON)
option(PBRT_DISABLE_LOGGING "Disable logs" OFF)
option(PBRT_DISABLE_PROFILING "Disable profiling" OFF)
option(PBRT_DISABLE_EXCEPTIONS "Disable exceptions" OFF)
option(PBRT_DISABLE_SIMD "Disable SIMD optimizations" OFF)

project(pbrt LANGUAGES CXX VERSION 0.0.1)

if (PBRT_BUILD_SHARED)
  set(PBRT_LIB_TYPE SHARED)
else()
  set(PBRT_LIB_TYPE STATIC)
endif()

add_library(pbrt ${PBRT_LIB_TYPE})

set_target_properties(
  pbrt
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    DEBUG_POSTFIX "d"
)

if(PBRT_DISABLE_LOGGING)
  message(STATUS "Logging is disabled")
endif()

if(NOT PBRT_DISABLE_SIMD)
  include (CheckCXXCompilerFlag)

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    check_cxx_compiler_flag("-mavx2" COMPILER_HAS_AVX2)
    if(COMPILER_HAS_AVX2)
      set(PBRT_HAS_AVX2 ON)
      target_compile_definitions(pbrt PUBLIC PBRT_HAS_AVX2)
      target_compile_options(pbrt PUBLIC -mavx2)
    endif()

    check_cxx_compiler_flag("-msse4.1" COMPILER_HAS_SSE41)
    if(COMPILER_HAS_SSE41)
      set(PBRT_HAS_SSE42 ON)
      target_compile_definitions(pbrt PUBLIC PBRT_HAS_SSE42)
      target_compile_options(pbrt PUBLIC -msse4.1)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    check_cxx_compiler_flag("/arch:AVX2" COMPILER_HAS_AVX2)
    if(COMPILER_HAS_AVX2)
      set(PBRT_HAS_AVX2 ON)
      target_compile_definitions(pbrt PUBLIC PBRT_HAS_AVX2)
      target_compile_options(pbrt PUBLIC /arch:AVX2)
    endif()

    check_cxx_compiler_flag("/arch:SSE2" COMPILER_HAS_SSE2)
    if(COMPILER_HAS_SSE2)
      set(PBRT_HAS_SSE42 ON)
      target_compile_definitions(pbrt PUBLIC PBRT_HAS_SSE42)
      target_compile_options(pbrt PUBLIC /arch:SSE2)
    endif()
  endif()
endif()

message(STATUS "SIMD: SSE4.1=${PBRT_HAS_SSE42}, AVX=${PBRT_HAS_AVX2}")

target_compile_definitions(
  pbrt
  PUBLIC
    $<$<BOOL:${PBRT_VERBOSE}>:PBRT_VERBOSE>
    $<$<BOOL:${PBRT_DISABLE_LOGGING}>:PBRT_DISABLE_LOGGING>
    $<$<BOOL:${PBRT_DISABLE_PROFILING}>:PBRT_DISABLE_PROFILING>
    $<$<BOOL:${PBRT_DISABLE_EXCEPTIONS}>:PBRT_DISABLE_EXCEPTIONS>
    $<$<BOOL:${PBRT_DISABLE_SIMD}>:PBRT_DISABLE_SIMD>

    # build definitions
    $<$<CONFIG:Debug>:PBRT_BUILD_DEBUG>
    $<$<CONFIG:Release,RelWithDebInfo,MinSizeRel>:PBRT_BUILD_RELEASE>

    # platform definitions
    $<$<PLATFORM_ID:Windows>:PBRT_PLATFORM_WINDOWS>

    # compiler definitions
    $<$<CXX_COMPILER_ID:MSVC>:PBRT_COMPILER_MSVC>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:PBRT_COMPILER_GCC>

    $<$<BOOL:${PBRT_BUILD_SHARED}>:PBRT_BUILD_SHARED>
  PRIVATE
    $<$<BOOL:${PBRT_BUILD_SHARED}>:PBRT_EXPORTS>
)

target_compile_options(
  pbrt
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive /Zc:__cplusplus /Zc:preprocessor>
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

find_package(EnTT CONFIG REQUIRED)

target_link_libraries(
  pbrt
  PUBLIC
    EnTT::EnTT
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

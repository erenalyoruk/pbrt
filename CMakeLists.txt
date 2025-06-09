cmake_minimum_required(VERSION 3.24)

option(PBRT_BUILD_SHARED "Build shared library" ON)
option(PBRT_BUILD_TESTS "Build tests" ON)
option(PBRT_DISABLE_ASSERTIONS "Disable assertions" OFF)
option(PBRT_DISABLE_EXCEPTIONS "Disable exceptions" ON)
option(PBRT_DISABLE_SIMD "Disable SIMD optimizations" OFF)
option(PBRT_VERBOSE "Enable verbose output" ON)

project(pbrt VERSION 0.1.1 LANGUAGES CXX)

if (PBRT_BUILD_SHARED)
  set(PBRT_LIB_TYPE SHARED)
else()
  set(PBRT_LIB_TYPE STATIC)
endif()

add_library(pbrt ${PBRT_LIB_TYPE})

if(NOT PBRT_DISABLE_SIMD)
  include(CheckCXXCompilerFlag)

  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    check_cxx_compiler_flag("-mavx2" COMPILER_HAS_AVX2)
    check_cxx_compiler_flag("-mfma" COMPILER_HAS_FMA)
    check_cxx_compiler_flag("-msse4.1" COMPILER_HAS_SSE41)

    if(COMPILER_HAS_AVX2)
      target_compile_options(pbrt PUBLIC -mavx2)
    endif()
    if(COMPILER_HAS_FMA)
      target_compile_options(pbrt PUBLIC -mfma)
    endif()
    if(COMPILER_HAS_SSE41)
      target_compile_options(pbrt PUBLIC -msse4.1)
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    check_cxx_compiler_flag("/arch:AVX2" COMPILER_HAS_AVX2)
    if(COMPILER_HAS_AVX2)
      target_compile_options(pbrt PUBLIC /arch:AVX2)
    endif()
  endif()
endif()

set_target_properties(
  pbrt
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    DEBUG_POSTFIX "d"
)

target_compile_definitions(
  pbrt
  PUBLIC
    $<$<BOOL:${PBRT_DISABLE_ASSERTIONS}>:PBRT_DISABLE_ASSERTIONS>
    $<$<BOOL:${PBRT_DISABLE_EXCEPTIONS}>:PBRT_DISABLE_EXCEPTIONS>
    $<$<BOOL:${PBRT_DISABLE_SIMD}>:PBRT_DISABLE_SIMD>
    $<$<BOOL:${PBRT_VERBOSE}>:PBRT_VERBOSE>

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

find_path(SIMDE_INCLUDE_DIRS "simde/arm/neon.h")
target_include_directories(pbrt PUBLIC "${SIMDE_INCLUDE_DIRS}")

add_subdirectory("src")

if (PBRT_BUILD_TESTS)
  enable_testing()
  add_subdirectory("tests")
endif()

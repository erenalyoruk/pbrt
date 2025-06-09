add_executable(pbrt_tests)

set_target_properties(
  pbrt_tests
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(
  pbrt
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive /Zc:__cplusplus /Zc:preprocessor>
)

target_sources(
  pbrt_tests
  PRIVATE
    "main.cpp"
    "test_assert.cpp"
    "test_types.cpp"
)

add_subdirectory("math")

find_package(GTest CONFIG REQUIRED)

target_link_libraries(
  pbrt_tests
  PRIVATE
    pbrt
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(pbrt_tests)

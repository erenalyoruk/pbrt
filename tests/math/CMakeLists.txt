target_sources(
  pbrt_tests
  PRIVATE
    "test_constants.cpp"
    "test_utility.cpp"
    "test_traits.cpp"
    "test_vector_traits.cpp"
)

add_subdirectory("vector")

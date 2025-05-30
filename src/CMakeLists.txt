target_sources(
  pbrt
  PRIVATE
    "assertions.cpp"
)

add_subdirectory("debug")
add_subdirectory("logging")
add_subdirectory("ecs")
add_subdirectory("profiling")

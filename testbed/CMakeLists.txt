add_executable(pbrt_testbed)

set_target_properties(
  pbrt_testbed
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>"
)

target_sources(
  pbrt_testbed
  PRIVATE
    "main.cpp"
)

target_link_libraries(
  pbrt_testbed
  PRIVATE
    pbrt
)

find_package(Doxygen QUIET)

if(NOT DOXYGEN_FOUND)
  message(STATUS "Doxygen not found, downloading prebuilt binary...")

  set(DOXYGEN_VERSION "1.13.2")
  string(REPLACE "." "_" DOXYGEN_VERSION_TAG "${DOXYGEN_VERSION}")
  set(DOXYGEN_BASE_URL "https://github.com/doxygen/doxygen/releases/download/Release_${DOXYGEN_VERSION_TAG}")

  if (WIN32)
    set(DOXYGEN_ARCHIVE "doxygen-${DOXYGEN_VERSION}.windows.x64.bin.zip")
    set(DOXYGEN_BIN_DIR "${CMAKE_BINARY_DIR}/doxygen/src/doxygen")
    set(DOXYGEN_BIN_PATH "${DOXYGEN_BIN_DIR}/doxygen.exe")

    set(DOXYGEN_EXECUTABLE "${DOXYGEN_BIN_PATH}" CACHE STRING "Path to prebuilt doxygen executable" FORCE)
  else()
    message(FATAL_ERROR "Prebuilt Doxygen download not configured for this platform")
  endif()

  include(ExternalProject)
  ExternalProject_Add(
    doxygen
    PREFIX "${CMAKE_BINARY_DIR}/doxygen"
    URL "${DOXYGEN_BASE_URL}/${DOXYGEN_ARCHIVE}"
    DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/doxygen"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
  )
endif()

if (DOXYGEN_FOUND OR DOXYGEN_EXECUTABLE)
  set(DOXYGEN_INPUT "${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src")
  set(DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/docs")

  configure_file("${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)

  add_custom_target(
    doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
else()
  message(STATUS "Doxygen not found, API documentation will not be generated.")
endif()

find_package(Doxygen QUIET)

set(DOXYGEN_VERSION "1.13.2")
string(REPLACE "." "_" DOXYGEN_VERSION_TAG "${DOXYGEN_VERSION}")
set(DOXYGEN_BASE_URL "https://github.com/doxygen/doxygen/releases/download/Release_${DOXYGEN_VERSION_TAG}")
set(DOXYGEN_INPUT "${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src")
set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs")

if (DOXYGEN_FOUND)
  set(DOXYGEN_EXECUTABLE_PATH ${DOXYGEN_EXECUTABLE})
  set(HAS_DOXYGEN TRUE)
else()
  if (WIN32)
    set(DOXYGEN_ARCHIVE "doxygen-${DOXYGEN_VERSION}.windows.x64.bin.zip")
    set(DOXYGEN_BIN_DIR "${CMAKE_BINARY_DIR}/doxygen/src/doxygen_bin")
    set(DOXYGEN_BIN_PATH "${DOXYGEN_BIN_DIR}/doxygen.exe")

    include(ExternalProject)
    ExternalProject_Add(
      doxygen_bin
      PREFIX "${CMAKE_BINARY_DIR}/doxygen"
      URL "${DOXYGEN_BASE_URL}/${DOXYGEN_ARCHIVE}"
      DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/doxygen"
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      LOG_DOWNLOAD ON
    )

    add_custom_target(pbrt_doc ALL
      DEPENDS doxygen_bin
      COMMAND ${DOXYGEN_BIN_PATH} "${CMAKE_BINARY_DIR}/Doxyfile"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
      COMMENT "Generating API documentation with downloaded Doxygen"
      VERBATIM
    )

    set(DOXYGEN_EXECUTABLE_PATH ${DOXYGEN_BIN_PATH})
    set(HAS_DOXYGEN TRUE)
  else()
    message(WARNING "Prebuilt Doxygen not supported on this platform, skipping docs.")
  endif()
endif()

if (HAS_DOXYGEN)
  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/Doxyfile.in"
    "${CMAKE_BINARY_DIR}/Doxyfile"
    @ONLY
  )
endif()

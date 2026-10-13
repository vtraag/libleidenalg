cmake_minimum_required (VERSION 3.23)

# Add etc/cmake to CMake's search path so we can put our private stuff there
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/etc/cmake)

include(version)

project(libleidenalg VERSION ${PACKAGE_VERSION_BASE})

# Default to an optimised build. Without this, single-config generators
# (e.g. Makefiles, as used by the wheel build scripts) compile with no
# optimisation flags at all (-O0), which makes the library several times slower.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(packaging)

find_package(igraph 1 CONFIG REQUIRED)

set_target_properties(igraph::igraph PROPERTIES
  INTERFACE_LINK_LIBRARIES ""
)

# Set C++ standard version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set default symbol visibility to hidden
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Expose the BUILD_SHARED_LIBS option in the ccmake UI
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_subdirectory(src)
add_subdirectory(include)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/libleidenalgConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/libleidenalgConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libleidenalg"
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/libleidenalgConfigVersion.cmake"
  COMPATIBILITY SameMinorVersion)
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/libleidenalgConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/libleidenalgConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libleidenalg"
  COMPONENT dev
)

export(
  TARGETS libleidenalg
  FILE "${PROJECT_BINARY_DIR}/libleidenalgTargets.cmake")
export(PACKAGE libleidenalg)

 install(
  TARGETS libleidenalg
  EXPORT libleidenalgTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  FILE_SET headers
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libleidenalg" COMPONENT dev
)
install(
  EXPORT libleidenalgTargets
  FILE libleidenalgTargets.cmake
  NAMESPACE libleidenalg::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/libleidenalg"
  )

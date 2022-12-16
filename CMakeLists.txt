cmake_minimum_required (VERSION 3.23)

project(libleidenalg VERSION 0.9.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(igraph 0.10 CONFIG REQUIRED)

# Set default symbol visibility to hidden
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_subdirectory(src)
add_subdirectory(include)

configure_package_config_file(libleidenalgConfig.cmake.in "${PROJECT_BINARY_DIR}/libleidenalgConfig.cmake"
INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/libleidenalg/cmake"
PATH_VARS CMAKE_INSTALL_INCLUDEDIR)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/libleidenalgConfigVersion.cmake" COMPATIBILITY SameMinorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libleidenalgConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/libleidenalgConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/libleidenalg/cmake"
        COMPONENT dev)

export(TARGETS libleidenalg FILE "${PROJECT_BINARY_DIR}/libleidenalgTargets.cmake")
export(PACKAGE libleidenalg)

 install(TARGETS libleidenalg
  EXPORT libleidenalgTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  FILE_SET headers
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libleidenalg"
    COMPONENT dev)
install(EXPORT libleidenalgTargets DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/libleidenalg/cmake" FILE libleidenalgTargets.cmake)

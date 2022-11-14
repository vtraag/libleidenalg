cmake_minimum_required (VERSION 3.23)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(leidenalg VERSION 0.9.0)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(igraph CONFIG REQUIRED)

add_subdirectory(src/leidenalg)
add_subdirectory(include)


export(TARGETS leidenalg FILE "${PROJECT_BINARY_DIR}/libleidenalgTargets.cmake")
export(PACKAGE leidenalg)

# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(libleidenalgConfig.cmake.in
  "${PROJECT_BINARY_DIR}/libleidenalgConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${PROJECT_CMAKE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
configure_file(libleidenalgConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libleidenalgConfig.cmake" @ONLY)
# ... for both
write_basic_package_version_file("${PROJECT_BINARY_DIR}/libleidenalgConfigVersion.cmake" COMPATIBILITY SameMinorVersion)


install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libleidenalgConfig.cmake"
  "${PROJECT_BINARY_DIR}/libleidenalgConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/libleidenalg" COMPONENT dev)

 install(TARGETS leidenalg
  EXPORT leidenalgTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIB}" COMPONENT shlib
  FILE_SET headers
    DESTINATION include/libleidenalg
    COMPONENT dev)
install(EXPORT leidenalgTargets DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/libleidenalg FILE leidenalgTargets.cmake)
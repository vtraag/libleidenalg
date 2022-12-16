add_library(libleidenalg SHARED CPMVertexPartition.cpp
GraphHelper.cpp
LinearResolutionParameterVertexPartition.cpp
ModularityVertexPartition.cpp
MutableVertexPartition.cpp
Optimiser.cpp
RBConfigurationVertexPartition.cpp
RBERVertexPartition.cpp
ResolutionParameterVertexPartition.cpp
SignificanceVertexPartition.cpp
SurpriseVertexPartition.cpp)

set_target_properties(libleidenalg PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libleidenalg PROPERTIES SOVERSION 1)

# Generate export header include file
include(GenerateExportHeader)
generate_export_header(libleidenalg
  STATIC_DEFINE LEIDENALG_STATIC
  EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/libleidenalg_export.h
)

# Make sure we install the export header
target_sources(libleidenalg
  PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS 
    ${PROJECT_BINARY_DIR}/include/
  FILES
    ${PROJECT_BINARY_DIR}/include/libleidenalg_export.h
)

# Make sure we can find include files
target_include_directories(
  libleidenalg
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)

target_link_libraries (libleidenalg PUBLIC igraph::igraph)

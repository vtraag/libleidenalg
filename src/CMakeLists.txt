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

target_sources(libleidenalg
  PUBLIC
  FILE_SET headers
  TYPE HEADERS
  FILES Optimiser.h 
        GraphHelper.h 
        CPMVertexPartition.h
        ModularityVertexPartition.h
        MutableVertexPartition.h
        RBConfigurationVertexPartition.h
        RBERVertexPartition.h
        ResolutionParameterVertexPartition.h
        SignificanceVertexPartition.h
        SurpriseVertexPartition.h)
set_target_properties(libleidenalg PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libleidenalg PROPERTIES SOVERSION 1)

# Generate export header include file
include(GenerateExportHeader)
generate_export_header(libleidenalg
  STATIC_DEFINE LEIDENALG_STATIC
  EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/libleidenalg_export.h
)

# Make sure we can find libleidenalg_export.h
target_include_directories(
  libleidenalg
  PUBLIC
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)

target_link_libraries (libleidenalg PUBLIC igraph::igraph)

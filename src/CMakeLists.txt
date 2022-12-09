add_library(leidenalg SHARED CPMVertexPartition.cpp
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

target_sources(leidenalg
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
set_target_properties(leidenalg PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(leidenalg PROPERTIES SOVERSION 1)
if(WIN32 AND NOT CYGWIN)
  set_target_properties(leidenalg PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

target_link_libraries (leidenalg PUBLIC igraph::igraph)

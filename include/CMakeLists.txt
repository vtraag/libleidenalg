target_sources(libleidenalg
  PUBLIC
  FILE_SET headers
  TYPE HEADERS
  BASE_DIRS 
    ${PROJECT_SOURCE_DIR}/include/
  FILES 
    Optimiser.h 
    GraphHelper.h 
    CPMVertexPartition.h
    ModularityVertexPartition.h
    MutableVertexPartition.h
    RBConfigurationVertexPartition.h
    RBERVertexPartition.h
    ResolutionParameterVertexPartition.h
    SignificanceVertexPartition.h
    SurpriseVertexPartition.h
  )

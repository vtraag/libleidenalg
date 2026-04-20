cmake_minimum_required (VERSION 3.23)

project(example)

find_package(igraph 1 CONFIG REQUIRED)
find_package(libleidenalg 0.12 CONFIG REQUIRED)

add_executable(
    example
    example.cpp
)

target_link_libraries(example PUBLIC igraph::igraph)
target_link_libraries(example PUBLIC libleidenalg::libleidenalg)

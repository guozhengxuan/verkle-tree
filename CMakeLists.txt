cmake_minimum_required(VERSION 3.14)

project(verkle_tree)
set(CMAKE_CXX_STANDARD 20)

set(VERKLE_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH ${VERKLE_CMAKE_DIR})

include(ProjectBLST)

add_subdirectory(utilities)

add_subdirectory(bandersnatch)
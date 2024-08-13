cmake_minimum_required(VERSION 3.10)

project(bandersnatch)

include(ExternalProject)

ExternalProject_Add(
        blst
        GIT_REPOSITORY https://github.com/supranational/blst.git
        GIT_TAG 52cc60d78591a56abb2f3d0bd1cdafc6ba242997
        PREFIX ${CMAKE_BINARY_DIR}/blst
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${CMAKE_BINARY_DIR}/blst/src/blst/build.sh
        BUILD_IN_SOURCE 0
        INSTALL_COMMAND ""
)

ExternalProject_Get_Property(blst SOURCE_DIR)
ExternalProject_Get_Property(blst BINARY_DIR)
set(BLST_LIB ${BINARY_DIR}/libblst.a)
set(BLST_INCLUDE_DIR ${SOURCE_DIR}/bindings)

include_directories(${BLST_INCLUDE_DIR})

add_library(bandersnatch bandersnatch/bandersnatch.cpp)

add_dependencies(bandersnatch blst)
target_link_libraries(bandersnatch PRIVATE ${BLST_LIB})
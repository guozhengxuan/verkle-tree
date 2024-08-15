cmake_minimum_required(VERSION 3.10)
project(bandersnatch)
set(CMAKE_CXX_STANDARD 20)

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
set(BLST_INCLUDE_DIR ${SOURCE_DIR}/bindings)
ExternalProject_Get_Property(blst BINARY_DIR)
set(BLST_LIB ${BINARY_DIR}/libblst.a)

file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(bandersnatch ${SRCS})
target_include_directories(bandersnatch PUBLIC ${BLST_INCLUDE_DIR})
add_dependencies(bandersnatch blst)
target_link_libraries(bandersnatch PRIVATE ${BLST_LIB})

if (TESTS)
    enable_testing()
    set(CTEST_OUTPUT_ON_FAILURE TRUE)
    add_subdirectory(test)
endif()
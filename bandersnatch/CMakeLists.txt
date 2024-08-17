cmake_minimum_required(VERSION 3.10)

project(bandersnatch)

file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

add_library(bandersnatch ${SRCS} ${HEADERS})
target_link_libraries(bandersnatch PRIVATE blst)

# if (TESTS)
#     enable_testing()
#     set(CTEST_OUTPUT_ON_FAILURE TRUE)
    add_subdirectory(test)
# endif()

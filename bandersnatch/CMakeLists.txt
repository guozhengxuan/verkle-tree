cmake_minimum_required(VERSION 3.14)

project(bandersnatch)

file(GLOB_RECURSE SRCS bandersnatch/*.cpp)
file(GLOB_RECURSE HEADERS bandersnatch/*.h)

add_library(bandersnatch ${SRCS} ${HEADERS})
target_link_libraries(bandersnatch PUBLIC blst)

if (TESTS)
    enable_testing()
    set(CTEST_OUTPUT_ON_FAILURE TRUE)
    add_subdirectory(test)
endif()
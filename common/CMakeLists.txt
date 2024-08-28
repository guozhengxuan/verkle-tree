project(common)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HEADERS *.h)

add_library(common ${SRCS} ${HEADERS})
target_include_directories(common PUBLIC .)
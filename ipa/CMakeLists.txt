project(ipa)

file(GLOB_RECURSE SRCS ipa/*.cpp)
file(GLOB_RECURSE HEADERS ipa/*.h)

add_library(ipa ${SRCS} ${HEADERS})
add_dependencies(ipa bandersnatch utilities common)
target_include_directories(ipa PUBLIC .)
target_link_libraries(ipa PUBLIC bandersnatch utilities common)

if (TESTS)
    enable_testing()
    set(CTEST_OUTPUT_ON_FAILURE TRUE)
    add_subdirectory(test)
endif()
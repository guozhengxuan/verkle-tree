project(ipa)

file(GLOB_RECURSE SRCS ipa/*.cpp)
file(GLOB_RECURSE HEADERS ipa/*.h)

add_library(ipa ${SRCS} ${HEADERS})
add_dependencies(ipa bandersnatch utilities)
target_include_directories(ipa PUBLIC ../bandersnatch ipa)
target_link_libraries(ipa PUBLIC bandersnatch utilities)

if (TESTS)
    enable_testing()
    set(CTEST_OUTPUT_ON_FAILURE TRUE)
    add_subdirectory(test)
endif()
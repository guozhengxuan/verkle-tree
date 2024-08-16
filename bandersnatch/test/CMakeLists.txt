file(GLOB_RECURSE TEST_SRCS "*.cpp")



add_executable(bandersnatch-test ${TEST_SRCS})
target_include_directories(bandersnatch-test PRIVATE .)
target_link_libraries(bandersnatch-test bandersnatch Boost::UnitTestFramework)
# add_test(NAME test-bandersnatch WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND bandersnatch-test)

file(GLOB_RECURSE TEST_SRCS "*.cpp")

add_executable(bandersnatch-test ${TEST_SRCS})
target_include_directories(bandersnatch-test PRIVATE .)

find_package(boost_unit_test_framework CONFIG REQUIRED)
find_package(boost_log CONFIG REQUIRED)

target_link_libraries(bandersnatch-test bandersnatch Boost::unit_test_framework Boost::log)
# add_test(NAME test-bandersnatch WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND bandersnatch-test)

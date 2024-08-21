file(GLOB_RECURSE TEST_SRCS "*.cpp")

add_executable(bandersnatch-test ${TEST_SRCS})
target_include_directories(bandersnatch-test PRIVATE . ..)

find_package(boost_unit_test_framework CONFIG REQUIRED)

target_link_libraries(bandersnatch-test PRIVATE utilities bandersnatch Boost::unit_test_framework)
add_test(NAME test-bandersnatch WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND bandersnatch-test)

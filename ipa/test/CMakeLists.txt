file(GLOB_RECURSE TEST_HEADERS "*.h")
file(GLOB_RECURSE TEST_SRCS "*.cpp")

add_executable(ipa-test ${TEST_HEADERS} ${TEST_SRCS})
target_include_directories(ipa-test PRIVATE .)

find_package(boost_unit_test_framework CONFIG REQUIRED)

target_link_libraries(ipa-test PRIVATE ipa Boost::unit_test_framework)
add_test(NAME test-bandersnatch WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COMMAND ipa-test)
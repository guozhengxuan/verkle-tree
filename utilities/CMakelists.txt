project(utilities)

find_package(boost_unit_test_framework CONFIG REQUIRED)
find_package(boost_filesystem CONFIG REQUIRED)

add_library(utilities INTERFACE)
target_include_directories(utilities INTERFACE .)
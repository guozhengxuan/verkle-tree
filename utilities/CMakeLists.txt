project(utilities)

find_package(OpenSSL CONFIG REQUIRED)
find_package(boost_unit_test_framework CONFIG REQUIRED)
find_package(boost_filesystem CONFIG REQUIRED)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HEADERS *.h)

add_library(utilities ${SRCS} ${HEADERS})
target_include_directories(utilities PUBLIC .)
target_link_libraries(utilities PUBLIC OpenSSL::Crypto)
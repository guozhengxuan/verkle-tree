cmake_minimum_required(VERSION 3.10)

set(VERKLE_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/verkle-cmake-scripts)
set(VERKLE_CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake" CACHE PATH "The cmake path for the library")
list(APPEND CMAKE_MODULE_PATH ${VERKLE_CMAKE_SCRIPTS_DIR} ${VERKLE_CMAKE_DIR})

# init hunter
include(HunterGate)
HunterGate(
        URL "https://github.com/FISCO-BCOS/hunter/archive/155dfdb7b5888493eefb656b3abcc026edb98730.tar.gz"
        SHA1 "a797abf35de1904ec089a9f517d847637731d467"
        FILEPATH "${VERKLE_CMAKE_DIR}/config.cmake"
)

project(verkle_tree)
set(CMAKE_CXX_STANDARD 20)

hunter_add_package(Boost COMPONENTS unit_test_framework)
find_package(Boost CONFIG REQUIRED unit_test_framework)

add_subdirectory(bandersnatch)

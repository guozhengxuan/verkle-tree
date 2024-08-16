#------------------------------------------------------------------------------
# Set compile options for FISCO-BCOS.
# ------------------------------------------------------------------------------
# This file is part of FISCO-BCOS.
#
# FISCO-BCOS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FISCO-BCOS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
#
# (c) 2016-2018 fisco-dev contributors.
#------------------------------------------------------------------------------
# change-list
# 2018/09/05: yujiechen
# 1. add DEBUG flag
# 2. add FISCO_DEBUG definition when DEBUG flag has been set

macro(eth_default_option O DEF)
    if (DEFINED ${O})
        if (${${O}})
            set(${O} ON)
        else ()
            set(${O} OFF)
        endif()
    else ()
        set(${O} ${DEF})
    endif()
endmacro()

# common settings
set(MARCH_TYPE "-march=x86-64 -mtune=generic -fvisibility=hidden -fvisibility-inlines-hidden")
set(ETH_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(ETH_SCRIPTS_DIR ${ETH_CMAKE_DIR}/scripts)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    message(FATAL "The ${PROJECT_NAME} does not support compiling on 32-bit systems")
endif()

EXECUTE_PROCESS(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)

macro(configure_project)
    set(NAME ${PROJECT_NAME})
    # Default to RelWithDebInfo configuration if no configuration is explicitly specified.
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
    endif()

    eth_default_option(BUILD_SHARED_LIBS OFF)

    eth_default_option(BUILD_STATIC OFF)

    #ARCH TYPE
    eth_default_option(ARCH_NATIVE OFF)
    set(USE_LD_GOLD ON)
    # if aarch64 set arch_native on
    if("${ARCHITECTURE}" STREQUAL "aarch64" OR "${ARCHITECTURE}" STREQUAL "arm64")
        set(ARCH_NATIVE ON)
    endif()
    if(ARCH_NATIVE)
        if(APPLE)
            set(MARCH_TYPE "-mtune=generic -fvisibility=hidden -fvisibility-inlines-hidden")
        else()
            set(MARCH_TYPE "-march=native")
            set(USE_LD_GOLD OFF)
        endif()
        set(COMPILE_OPTIONS "native")
    endif()

    # unit tests
    eth_default_option(TESTS OFF)
    # mini demos
    eth_default_option(DEMO OFF)
    # tools
    eth_default_option(TOOL OFF)
    # code coverage
    eth_default_option(COVERAGE OFF)
    # hardware crypto sdf interface
    eth_default_option(USE_HSM_SDF OFF)
    if(USE_HSM_SDF)
        if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
            message(FATAL "${CMAKE_SYSTEM_NAME} ${ARCHITECTURE} does not support by hardware secure module")
        endif()
        add_definitions(-DFISCO_SDF)
        if(NOT DEFINED COMPILE_OPTIONS)
            set(COMPILE_OPTIONS "hsm_sdf")
        else()
            set(COMPILE_OPTIONS "${COMPILE_OPTIONS},hsm_sdf")
        endif()
    endif()

    #debug
    eth_default_option(DEBUG OFF)
    if (DEBUG)
        add_definitions(-DFISCO_DEBUG)
    endif()

    #perf
    eth_default_option(PROF OFF)
    if (PROF)
    	#add_definitions(-DPROF)
	endif()
    eth_default_option(WITH_URING OFF)
    eth_default_option(WITH_TBB ON)
    if(TESTS)
        set(WITH_TBB ON)
    endif()
    if(NOT WITH_TBB)
        if(NOT DEFINED COMPILE_OPTIONS)
            set(COMPILE_OPTIONS "without_tbb")
        else()
            set(COMPILE_OPTIONS "${COMPILE_OPTIONS},without_tbb")
        endif()
    endif()
    if (WITH_TBB)
        add_definitions(-DWITH_TBB)
    endif()

    # Define a matching property name of each of the "features".
    foreach(FEATURE ${ARGN})
        set(SUPPORT_${FEATURE} TRUE)
    endforeach()
    if(DISABLE_OBSERVER_WRITE_REQUEST)
        add_definitions(-DDISABLE_OBSERVER_WRITE_REQUEST)
        if(NOT DEFINED COMPILE_OPTIONS)
            set(COMPILE_OPTIONS "disable_observer_write_request")
        else()
            set(COMPILE_OPTIONS "${COMPILE_OPTIONS},disable_observer_write_request")
        endif()
    endif()

    # CI Builds should provide (for user builds this is totally optional)
    # -DBUILD_NUMBER - A number to identify the current build with. Becomes TWEAK component of project version.
    # -DVERSION_SUFFIX - A string to append to the end of the version string where applicable.
    if (NOT DEFINED BUILD_NUMBER)
        # default is big so that local build is always considered greater
        # and can easily replace CI build for for all platforms if needed.
        # Windows max version component number is 65535
        set(BUILD_NUMBER 65535)
    endif()

    # Suffix like "-rc1" e.t.c. to append to versions wherever needed.
    if (NOT DEFINED VERSION_SUFFIX)
        set(VERSION_SUFFIX "")
    endif()

endmacro()

macro(print_config NAME)
    message("")
    message("------------------------------------------------------------------------")
    message("-- Configuring ${NAME} ${PROJECT_VERSION}${VERSION_SUFFIX}")
    message("------------------------------------------------------------------------")
    message("-- CMake              Cmake version and location       ${CMAKE_VERSION} (${CMAKE_COMMAND})")
    message("-- Compiler           C++ compiler version             ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    message("-- CMAKE_BUILD_TYPE   Build type                       ${CMAKE_BUILD_TYPE}")
    message("-- TARGET_PLATFORM    Target platform                  ${CMAKE_SYSTEM_NAME} ${ARCHITECTURE}")
    message("-- BUILD_STATIC       Build static                     ${BUILD_STATIC}")
    message("-- USE_HSM_SDF        Build SDF HSM                    ${USE_HSM_SDF}")
    message("-- DEMO               Build demos                      ${DEMO}")
    message("-- TOOL               Build tools                      ${TOOL}")
    message("-- COVERAGE           Build code coverage              ${COVERAGE}")
    message("-- TESTS              Build tests                      ${TESTS}")
    message("-- WITH_TBB           Build with tbb                   ${WITH_TBB}")
    message("-- APPLE              Apple Machine                    ${APPLE}")
    message("-- ARCH_NATIVE        Enable native code               ${ARCH_NATIVE}")
    message("-- DEBUG              Enable debug macro               ${DEBUG}")
    message("-- PROF               Enable gcc prof                  ${PROF}")
    message("-- DEFINE_FILTER_OWR  DISABLE_OBSERVER_WRITE_REQUEST   ${DISABLE_OBSERVER_WRITE_REQUEST}")
    message("-- WITH_URING         Compile with liburing            ${WITH_URING}")
    message("------------------------------------------------------------------------")
    message("")
endmacro()


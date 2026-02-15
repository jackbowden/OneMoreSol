###########################
# CMake Build Environment #
###########################
#
# You can set the following Environment variables:
#
# CMAKE_MODULE_PATH to the search path for cmake modules (e.g., SFML.cmake)
# CMAKE_BUILD_TYPE to override the default build type. Valid options are 'release', 'profile' and 'debug'
#
#########################################

# Minimum Cmake Version
cmake_minimum_required(VERSION 3.16)

# Project name
project(csci437)

#####################
# Setup Environment #
#####################
# set to include custom modules
set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} ${csci437_SOURCE_DIR}/cmake)

# set build type if specified by environment
if((NOT CMAKE_BUILD_TYPE) AND (NOT $ENV{CMAKE_BUILD_TYPE} STREQUAL ""))
    set(CMAKE_BUILD_TYPE $ENV{CMAKE_BUILD_TYPE})
endif()

# Set include directories
include_directories(${csci437_SOURCE_DIR}/include)
include_directories(${csci437_SOURCE_DIR}/include/Guns)
include_directories(${csci437_SOURCE_DIR}/include/Korat)
include_directories(${csci437_SOURCE_DIR}/include/Bullets)



# Get CPP files

#file(GLOB SRC src/*.cpp)
#file(GLOB_RECURSE SRC src/*.cpp)

#file(GLOB_RECURSE SRC ${csci437_SOURCE_DIR}/**/*.cpp)

file(GLOB SRC "src/Bullets/*.cpp" "src/Korat/*.cpp" "src/Guns/*.cpp" "src/*.cpp")
#file(GLOB SRC src/Guns/*cpp)
#file(GLOB SRC src/Bullets*cpp)
#file(GLOB SRC src/Korat/*cpp)
#file(GLOB SRC src/*cpp)

# Get executable files
file(GLOB EXECLIST src/main.cpp)

#file(COPY ${csci437_SOURCE_DIR}/src/Korat DESTINATION ${csci437_SOURCE_DIR}/src)
#file(COPY ${csci437_SOURCE_DIR}/src/Guns DESTINATION ${csci437_SOURCE_DIR}/src)
#file(COPY ${csci437_SOURCE_DIR}/src/Bullets DESTINATION ${csci437_SOURCE_DIR}/src)


#################################################
#                                               #
# ADD CUSTOM LIBRARIES HERE.                    #
#                                               #
# Example BOOST:                                #
#  find_package(Boost REQUIRED)                 #
#  include_directories(${Boost_INCLUDE_DIRS})   #
#  link_libraries(${Boost_LIBRARIES})           #
#################################################

# Find libserialport for arcade coin acceptor (Linux/production builds only)
if(NOT APPLE)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(SERIALPORT libserialport)
        if(SERIALPORT_FOUND)
            include_directories(${SERIALPORT_INCLUDE_DIRS})
            link_directories(${SERIALPORT_LIBRARY_DIRS})
            message("-- Found libserialport for coin acceptor support")
        else()
            message(WARNING "libserialport not found. Coin acceptor will not function on production hardware.")
            message(WARNING "Install with: sudo apt-get install libserialport-dev (Debian/Ubuntu)")
        endif()
    endif()
else()
    message("-- macOS detected: Using keyboard coin reader for development")
endif()


#find_package(Guns REQUIRED)                 
#include_directories(${Guns_INCLUDE_DIRS})   
#link_libraries(${Guns_LIBRARIES}) 

#find_package(Guns REQUIRED)
#include_directories(${csci437_SOURCE_DIR}/include/Guns)
#link_directories(${csci437_SOURCE_DIR}/src/Guns)

#include_directories(${csci437_SOURCE_DIR}/include/Bullets)
#link_directories(${csci437_SOURCE_DIR}/src/Bullets)
#include_directories(${csci437_SOURCE_DIR}/include/Korat)
#link_directories(${csci437_SOURCE_DIR}/src/Korat)



#############
# Find SFML #
#############
# NOTE: This project requires SFML 2.x
# If you have SFML 3.x installed, downgrade: brew install sfml@2
find_package(SFML 2 COMPONENTS graphics window system audio network CONFIG QUIET)

if(NOT SFML_FOUND)
  find_package(SFML 2 REQUIRED COMPONENTS graphics window system audio network)
endif()

set(ONEMORESOL_SFML_LIBS)
if(TARGET SFML::Graphics)
  list(APPEND ONEMORESOL_SFML_LIBS SFML::Graphics SFML::Window SFML::System SFML::Audio SFML::Network)
elseif(TARGET sfml-graphics)
  list(APPEND ONEMORESOL_SFML_LIBS sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
else()
  include_directories(${SFML_INCLUDE_DIR})
  list(APPEND ONEMORESOL_SFML_LIBS ${SFML_LIBRARIES})
endif()

if(NOT SFML_FOUND)
  # SFML not found
  message(FATAL_ERROR " * Make sure LIB includes the directory where the SFML libraries are installed.\n * Make sure INCLUDE includes the directory where the SFML header files are installed.")	
endif()

###############
# C++ Options #
###############
# Enable C++17 (required for SFML 3.x)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# determine build type
# 1) use build type if specified by the user.
# 2) if in-source building, use default (debug) build type.
# 3) if out-source building, determine based on the build directory's name which build type. 
#    If build directory is called 'Release' or 'Profile' then use the 'Release' and 'Profile' 
#    build type respectively, otherwise use 'debug'.
if(CMAKE_BUILD_TYPE)
  string(TOUPPER "${CMAKE_BUILD_TYPE}" TYPE)
  message("-- Using user specified build type: ${TYPE}")
elseif(${csci437_SOURCE_DIR} STREQUAL ${csci437_BINARY_DIR})
  set(CMAKE_BUILD_TYPE Debug)
  message("-- In-source building. Build type set to: Debug")
else()
  get_filename_component(TYPE ${csci437_BINARY_DIR} NAME)
  string(TOUPPER "${TYPE}" TYPE)
  if(${TYPE} STREQUAL "RELEASE")
    set(CMAKE_BUILD_TYPE Release)
  elseif(${TYPE} STREQUAL "PROFILE")
    set(CMAKE_BUILD_TYPE Profile)
  else()
    set(CMAKE_BUILD_TYPE Debug)
 endif()
 message("-- Out-source building. Build type set to: ${TYPE}")
endif()

# Handle non-default build type 'Profile'
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
if(${CMAKE_BUILD_TYPE} STREQUAL "PROFILE")
  set(CMAKE_BUILD_TYPE DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
endif()

#######################
# Set Compile Targets #
#######################
# src library (all CPP files in 'src' dir)
if(NOT SRC STREQUAL "")
  get_filename_component(LIBNAME ${csci437_SOURCE_DIR} NAME)
  set(LIBNAME "${LIBNAME}_core")
  add_library(${LIBNAME} ${SRC})
  target_link_libraries(${LIBNAME} PUBLIC ${ONEMORESOL_SFML_LIBS})
  
  # Link libserialport on non-Apple platforms if available
  if(NOT APPLE AND SERIALPORT_FOUND)
    target_link_libraries(${LIBNAME} PUBLIC ${SERIALPORT_LIBRARIES})
  endif()
endif()

# executables (any CPP file in 'bin' dir)
foreach(EXEC ${EXECLIST})
  get_filename_component(EXECNAME ${EXEC} NAME_WE)
  add_executable(${EXECNAME} ${EXEC})

  if(NOT SRC STREQUAL "")
    target_link_libraries(${EXECNAME} LINK_PUBLIC ${LIBNAME})
  endif()

  target_link_libraries(${EXECNAME} LINK_PUBLIC ${ONEMORESOL_SFML_LIBS})

  message("-- Adding executable: ${EXECNAME}")
endforeach(EXEC)


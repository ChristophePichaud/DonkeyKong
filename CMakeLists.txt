cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

# Project name
set(PROJECT_NAME DonkeyKong)

# Output Variables
#set(OUTPUT_DEBUG Debug/bin)
#set(OUTPUT_RELEASE Release/bin)

# Library
set(LIBRARY_PATH library)

# SFML Lib
set(SFML_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_PATH}/SFML-2.5.1)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/library/cmake_modules")

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} CXX)

# Add index CMakeFiles.tkt from /src
include(${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/include/CMakeLists.txt)

# Add executable to build.
add_executable(${PROJECT_NAME}
        ${PROJECT_NAME_HEADERS}
        ${PROJECT_NAME_SOURCES})

# Include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

################ SFML ##################
#   --   Add library to project.  --   #
########################################

find_package(SFML 2 REQUIRED system window graphics network audio)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif ()

cmake_minimum_required(VERSION 3.1.0)

# Define library name and version
set(PARENT_PROJECT ${PROJECT_NAME})

project(imp) # Initialize PROJECT_xxx variables

set(IMP_VERSION_MAJOR     0)
set(IMP_VERSION_MINOR     1)
set(IMP_VERSION_REVISION  0)

set(CMAKE_CXX_STANDARD 14)

# compilation options
#option(BUILD_TESTS "Build the unit tests" OFF)


# Configure output directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)

# configure build modes
IF(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions("-DNDEBUG")
ENDIF()


# TODO: use [add_compile_warnings] in new CMake instead of this
# configure compiler options
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wpedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wpedantic")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX /EHsc")

    # disable MS warning about safer *_s functions
    add_compile_options(/wd4996)
endif()

# Generate and use config.h file
configure_file (
    ${PROJECT_SOURCE_DIR}/config.in
    ${PROJECT_BINARY_DIR}/include/imp/config
    @ONLY                                          # replace VAR with pattern @VAR@ only (not ${VAR})
)
add_definitions("-DHAVE_CONFIG_H")                 # define HAVE_CONFIG_H for project
include_directories(${PROJECT_BINARY_DIR}/include) # for config.h using

# Enumerate source files
set(SOURCES
    src/version.cpp
)

# Enumerate header files
set(HEADERS
    include/imp/version
    include/imp/common/matrix)


# define and export library
add_library(imp STATIC ${SOURCES} ${HEADERS})

# use linker for C++ (CXX)
set_target_properties(imp PROPERTIES LINKER_LANGUAGE CXX
                                     CXX_STANDARD 14
                                     CXX_STANDARD_REQUIRED ON)

# populate ex headers
target_include_directories(imp PUBLIC ${PROJECT_SOURCE_DIR}/include)

# initialize external libs
add_subdirectory(external)

# linking against libex and share its interface
target_link_libraries(imp PUBLIC ex)


# unit tests
#if (BUILD_TESTS)
#    add_subdirectory(test)
#    message(STATUS "Test building enabled")
#endif()

cmake_minimum_required(VERSION 3.16)
project(CPURaytracer VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Output binary location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)

# Gather source and header files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    ${INCLUDE_DIR}/*.h
    ${INCLUDE_DIR}/*.hpp
)

# Executable target
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INCLUDE_DIR}            # Your own headers
        ${EXTERNAL_INCLUDE_DIR}   # External libs headers
)

# Debug macro
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG_BUILD>
)

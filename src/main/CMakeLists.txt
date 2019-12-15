cmake_minimum_required (VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Looking for Boost
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# Looking for json11
message(STATUS "Looking for json11 lib")
find_library(JSON11_LIBRARY
        NAMES json11
        HINTS ${CMAKE_SOURCE_DIR}/lib/json11)


# Setting up include directories
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(recreate-tool
        recreate-tool.cpp)

target_link_libraries(recreate-tool
        stdc++fs
        ${JSON11_LIBRARY}
        ${Boost_LIBRARIES})

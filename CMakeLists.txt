cmake_minimum_required (VERSION 2.6)

project (recreate-tool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIR})

add_executable(recreate-tool recreate-tool.cpp)

target_link_libraries(recreate-tool ${Boost_LIBRARIES})
target_link_libraries(recreate-tool stdc++fs)


find_package(Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIR})

add_executable(recreate-tool recreate-tool.cpp)

target_link_libraries(recreate-tool ${Boost_LIBRARIES})
target_link_libraries(recreate-tool stdc++fs)
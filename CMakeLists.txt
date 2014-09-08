project(api-generator)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11 -g)

set(SRC_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp
)

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory("${PROJECT_SOURCE_DIR}/model")
add_subdirectory("${PROJECT_SOURCE_DIR}/generator")

add_executable(${PROJECT_NAME} ${HDR_LIST} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} yaml-cpp)

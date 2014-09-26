project(api-generator)
cmake_minimum_required(VERSION 2.8)
add_definitions(-std=c++11 -g)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.51.0 COMPONENTS regex)

if(Boost_FOUND)
    set(SRC_LIST
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

    include_directories(${Boost_INCLUDE_DIRS})

    include_directories(${PROJECT_SOURCE_DIR})
    add_subdirectory("${PROJECT_SOURCE_DIR}/model")
    add_subdirectory("${PROJECT_SOURCE_DIR}/parser")
    add_subdirectory("${PROJECT_SOURCE_DIR}/transformer")
    add_subdirectory("${PROJECT_SOURCE_DIR}/formatter")
    add_subdirectory("${PROJECT_SOURCE_DIR}/compiler")

    add_executable(${PROJECT_NAME} ${HDR_LIST} ${SRC_LIST})

    target_link_libraries(${PROJECT_NAME} yaml-cpp)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

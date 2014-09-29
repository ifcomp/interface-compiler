cmake_minimum_required(VERSION 2.8)

project(EverbaseInterfaceCompiler)

add_definitions(-std=c++11 -g)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS regex filesystem system)

set(SRC
    StreamFilter.hpp
    Meta.hpp
    MultiStream.hpp
    IndexList.hpp
    CompilerSetup.hpp
    Config.hpp
    FormatToken.hpp

    Compiler.cpp
    Parser.cpp
    Transformer.cpp
    Formatter.cpp
    main.cpp
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    Components
    Model
    .
)

add_subdirectory(Components)
add_subdirectory(Model)

add_executable(compiler ${SRC})
target_link_libraries(compiler ${Boost_LIBRARIES} yaml-cpp)

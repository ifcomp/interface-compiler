cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(EverbaseInterfaceCompiler)

message ("BuildType: " ${CMAKE_BUILD_TYPE} )
if( CMAKE_C_COMPILER_ID MATCHES MSVC AND CMAKE_CXX_COMPILER_ID MATCHES MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX- /wd4100 /wd4101 /wd4510 /wd4610 /wd4512 /wd4514 /wd4250 /wd4820 /D_SCL_SECURE_NO_WARNINGS /D_WIN32_WINNT=0x0601 /D_WINSOCK_DEPRECATED_NO_WARNINGS" )
    set( CMAKE_GENERATOR_TOOLSET "v140" CACHE STRING "Platform Toolset" FORCE)
    add_definitions( -DBOOST_ALL_NO_LIB )
elseif(CMAKE_C_COMPILER_ID MATCHES GNU AND CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter --pedantic" )
elseif(CMAKE_C_COMPILER_ID MATCHES Clang AND CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter --pedantic" )
endif()

if( NOT(CMAKE_CXX_COMPILER_ID MATCHES MSVC) )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g" )
endif()

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost COMPONENTS regex filesystem system)

find_package(YamlCpp REQUIRED)

set(SRC
    StreamFilter.hpp
    Meta.hpp
    MultiStream.hpp
    IndexList.hpp
    CompilerSetup.hpp
    FormatToken.hpp

    Compiler.hpp
    Formatter.hpp
    Parser.hpp
    Transformer.hpp

    Compiler.cpp
    Parser.cpp
    Transformer.cpp
    Formatter.cpp
    main.cpp
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${YamlCpp_INCLUDE_DIRS}
    Components/Cpp
    Components/JavaScript
    Components
    Model
    .
)

add_subdirectory(Components)
add_subdirectory(Model)

add_executable(compiler ${SRC})
target_link_libraries(compiler ${Boost_LIBRARIES} ${YamlCpp_LIBRARIES})

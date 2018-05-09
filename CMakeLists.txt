cmake_minimum_required(VERSION 3.7)
project(cppmark)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


add_executable(tests
        src/main.cpp
        src/parser.cpp
        src/token.cpp
        src/rules.cpp
        src/renderer.cpp
        src/options.cpp)

add_executable(testEncoding
        src/testEncoding.cpp)
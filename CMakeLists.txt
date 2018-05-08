cmake_minimum_required(VERSION 3.7)
project(cppmark)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")


#add_executable(testLineMarks
#        test/testLineMarks.cpp
#        src/cppmark.cpp)

#add_executable(testTokens
#        test/testTokens.cpp
#        src/cppmark.cpp)

#add_executable(testRegex
#        src/main.cpp
#        src/block.cpp)

add_executable(tests
        src/main.cpp
        src/parser.cpp
        src/token.cpp
        src/rules.cpp
        src/renderer.cpp
        src/options.cpp)
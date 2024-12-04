cmake_minimum_required(VERSION 3.26)
project(SFML_fragment_shaders C)

set(CMAKE_C_STANDARD 11)
include_directories( ./ )
add_executable(SFML_fragment_shaders
        code/HelloWorld.c
        code/SimpleTest.c)

cmake_minimum_required(VERSION 3.26)
project(SFML_fragment_shaders C)

set(CMAKE_C_STANDARD 11)
include_directories( ./ )
add_executable(SFML_fragment_shaders
        test/HelloWorld.c
        test/SimpleTest.c
        test/testThree.c
        engine/maths/bbGPU_Interpolation.h
        engine/maths/bbGPU_Interpolation.c
        test/test_Kernels.c)

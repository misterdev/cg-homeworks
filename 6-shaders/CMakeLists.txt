cmake_minimum_required(VERSION 3.9)
project(6_shaders)

set(CMAKE_CXX_STANDARD 11)

add_executable(6_shaders
    BUMPMAP/src/bumptex.cpp
    common/common.h
    CUBE/src/cube.cpp
    CUBEMAP/src/cubetexshader.cpp
    CUBESHADE/src/cubesh.cpp
    CUBETEX/src/cubetex.cpp
    MORPH/src/morph.cpp
    NONPHOTO/src/nonphoto.cpp
    PARTICLE/src/particle.cpp
    PHONG/src/phong.cpp
    WAVE/src/wave.cpp)

cmake_minimum_required(VERSION 3.6)
project(6-shaders)

set(CMAKE_C_STANDARD 90)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

set(CMAKE_WARN_DEPRECATED FALSE)
ADD_DEFINITIONS(-Wno-deprecated-declarations)

set(SOURCE_FILES
		src/cubetexshader.cpp
		src/v.glsl
		src/f.glsl
        ../../common/common.h)

add_executable(6-cubemap ${SOURCE_FILES})

target_link_libraries(6-cubemap ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

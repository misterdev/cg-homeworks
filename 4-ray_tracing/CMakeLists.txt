cmake_minimum_required(VERSION 3.8)
project(4-ray_tracing)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES 
    src/main.cpp
    src/argparser.h
    src/bag.h
    src/boundingbox.cpp
    src/boundingbox.h
    src/camera.cpp
    src/camera.h
    src/edge.cpp
    src/edge.h
    src/face.cpp
    src/face.h
    src/glCanvas.cpp
    src/freeglut.h
    src/freeglut_ext.h
    src/freeglut_std.h
    src/glut.h
    src/hit.h
    src/material.cpp
    src/matrix.cpp
    src/mesh.cpp
    src/radiosity.cpp
    src/raytracer.cpp
    src/sphere.cpp
    src/ray.h
    src/raytree.cpp
    src/utils.h
    src/vectors.h
    src/vertex.h
    src/vertex_parent.h
)
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

#########################################################

set(CMAKE_WARN_DEPRECATED FALSE)
ADD_DEFINITIONS(-Wno-deprecated-declarations)

add_executable(4-ray_tracing ${SOURCE_FILES})
target_link_libraries(4-ray_tracing ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} -lm)
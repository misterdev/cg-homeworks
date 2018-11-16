cmake_minimum_required(VERSION 3.7)
project(4_ray_tracing)

set(CMAKE_CXX_STANDARD 11)

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

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_WARN_DEPRECATED FALSE)
ADD_DEFINITIONS(-Wno-deprecated-declarations)

set(SOURCE_FILES
        render/vs2010-lab-04/raytracing/raytracing.vcxproj
        render/vs2010-lab-04/raytracing/raytracing.vcxproj.filters
        render/vs2010-lab-04/raytracing/raytracing.vcxproj.user
        render/vs2010-lab-04/vs2008-lab-04.suo
        render/vs2010-lab-04/vs2010-lab-04.sdf
        render/vs2010-lab-04/vs2010-lab-04.sln
        render/vs2010-lab-04/vs2010-lab-04.suo
        render/argparser.h
        render/bag.h
        render/boundingbox.cpp
        render/boundingbox.h
        render/camera.cpp
        render/camera.h
        render/edge.cpp
        render/edge.h
        render/face.cpp
        render/face.h
        render/freeglut.h
        render/freeglut_ext.h
        render/freeglut_std.h
        render/glCanvas.cpp
        render/glCanvas.h
        render/glut.h
        render/hit.h
        render/main.cpp
        render/Makefile
        render/material.cpp
        render/material.h
        render/matrix.cpp
        render/matrix.h
        render/mesh.cpp
        render/mesh.h
        render/radiosity.cpp
        render/radiosity.h
        render/ray.h
        render/raytracer.cpp
        render/raytracer.h
        render/raytree.cpp
        render/raytree.h
        render/sphere.cpp
        render/sphere.h
        render/utils.h
        render/vectors.h
        render/vertex.h
        render/vertex_parent.h)

add_executable(4_ray_tracing ${SOURCE_FILES})

target_link_libraries(4_ray_tracing ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
cmake_minimum_required(VERSION 3.6)
project(2-model_viewer)

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
        src/main.c
        src/v3d.c
        src/v3d.h src/utils.h src/utils.c)
add_executable(2-model_viewer ${SOURCE_FILES})

target_link_libraries(2-model_viewer ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
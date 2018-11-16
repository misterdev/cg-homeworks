CC                = g++ -g -O3 -Wall -pedantic
INCLUDE_PATH      = -I/usr/X11R6/include -L/usr/local/include
LIB_PATH          = -L/usr/X11R6/lib -L/usr/local/lib
LIBS              = -lm -lGL -lGLU -lglut

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)  # Mac OS X
LIBS = -framework OpenGL -framework GLUT
endif

# ===============================================================

SRCS	= main.cpp matrix.cpp camera.cpp glCanvas.cpp mesh.cpp edge.cpp radiosity.cpp face.cpp raytree.cpp raytracer.cpp sphere.cpp material.cpp
OBJS	= $(SRCS:.cpp=.o)
EXE	= render

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE) 

depend:
	$(CC) $(INCLUDE_PATH) -E -M $(SRCS) > Makefile.depend

clean: 
	rm -f *~ *bak *.ob  *.o $(EXE) Makefile.depend

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) $(INCLUDE_PATH) -o $@ $(OBJS) $(LIB_PATH) $(LIBS) 

.cpp.o:
	$(CC) $(INCLUDE_PATH) $< -c -o $@

# ===============================================================

-include Makefile.depend

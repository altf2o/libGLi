STAT_INCLUDE=-I/usr/src/sys/sys

# This is the include path for our custom OpenGL image library.
GLI_INCLUDE=-I../include
GLI_LIB=-L../lib

# UPDATE 9/21/2009
# 	On main development platform OpenGL libraries are included in
#	/usr/include/GL now. 
#
# The include and library paths for OpenGL, GLU, GLUT etc...
GL_INCLUDE=-I/usr/include
GL_LIB=-L/usr/lib

# UPDATE 9/21/2009
# 	Same as above, zlib now included in just /usr/include
#
# The include and library paths for the zlib compression library
ZLIB_INCLUDE=-I/usr/include
ZLIB_LIB=-L/usr/lib

# Include paths
GLI_INC=$(GLI_INCLUDE) $(GL_INCLUDE) $(ZLIB_NCLUDE)
TGA_INC=$(GLI_INCLUDE) $(GL_INCLUDE) $(ZLIB_INCLUDE) $(STAT_INCLUDE)
TOOLS_INC=$(GLI_INCLUDE) $(GL_INCLUDE) $(ZLIB_INCLUDE) 

# Compiler flags
LIB_CFLAGS=-fPIC -shared
TOOLS_CFLAGS=-fomit-frame-pointer -Wall -pedantic
 
# Lib paths
GLI_LIBS=$(GL_LIB) $(ZLIB_LIB)
TGA_LIBS=$(GLI_LIB) $(GL_LIB) $(ZLIB_LIB)
TOOLS_LIBS=$(GLI_LIB) $(GL_LIB) $(ZLIB_LIB)

# Linker flags
GLI_LFLAGS=-lGL -lGLU -lglut -lz
TGA_LFLAGS=-lGLi -lGL -lGLU -lglut -lz
TOOLS_LFLAGS=-lGLi -lGL -lGLU -lglut -lz

GCC=gcc
DEBUG=-ggdb

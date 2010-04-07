# Author: 
#	Frank L. Morales II (altf2o)
# Purpose:
# 	This make file will build our libTGA.so shared object for FreeBSD
# Notes:
#	My development box uses FreeBSD 5.3 w/ standard include paths.
#	I added libglut from the ports collection, and compiled the zlib
#	library by hand. zlib by default for me was installed into:
#	/usr/local/include and /usr/local/lib. Please adjust the variables
#	below to properly reflect your systems paths. Also if you don't 
#	want to add another path to your ld.so.conf you'll need to export
#	the lib/ directory we use. Using the bash shell you can use:
#	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/<yourname>/
#	<path_to_libGLi/lib> This will only work for that one session in
# 	that one terminal. Add that line to your ~/.profile, or just add
# 	it to ld.so.conf and run ldconfig. Or you can simply copy all the
#	*.so files from our /lib/ directory, and put them in an already
#	known and searched for library path.
#

include ../Makefile.inc

#------------------#
#   Build options  #
#------------------#

all:
	@echo "/libTGA_src/Makefile"
	@echo "Building libTGA.so shared object (RELEASE)"
	@ $(GCC) $(TGA_INC) $(TGA_LIBS) $(LIB_CFLAGS) -s libTGA.c -o ../lib/libTGA.so \
	$(TGA_LFLAGS)
	@echo "Moving libTGA.so (RELEASE) to ../lib/"

debug:
	@echo "/libTGA_src/Makefile"
	@echo "Building libTGA.so shared object (DEBUG)"
	@ $(GCC) $(TGA_INC) $(TGA_LIBS) $(LIB_CFLAGS) $(DEBUG) libTGA.c -o \
	../lib/libTGA.so $(TGA_LFLAGS)
	@echo "Moving libTGA.so (DEBUG) to ../lib/"

clean:
	@echo "/libTGA_src/Makefile"
	@echo "Removing ../lib/libTGA.so"
	@ rm ../lib/libTGA.so

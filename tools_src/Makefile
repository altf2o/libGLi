#	Frank L. Morales II (altf2o)
# Purpose:
# 	This make file will build our various tools for FreeBSD
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

#-------------------#
#   Build options   #
#-------------------#
all:
	@echo ""
	@echo "/tools_src/Makefile"
	@echo "Building tools_gli_preview.c (RELEASE)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) -s tools_gli_preview.c -o \
	../tools_gli_preview $(TOOLS_LFLAGS)

	@echo "Building tools_textured_cube.c (RELEASE)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) -s tools_textured_cube.c -o \
	../tools_textured_cube $(TOOLS_LFLAGS)

	@echo "Building tools_convertTGAtoGLi.c (RELEASE)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) -s tools_convertTGAtoGLi.c -o \
	../tools_convertTGAtoGLi $(TOOLS_LFLAGS) -lTGA

	@echo "Building tools_image_manip.c (RELEASE)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) -s tools_image_manip.c -o \
	../tools_image_manip $(TOOLS_LFLAGS)
	
# NOTE: These files are technically already there, but printing this
# should remove any confusion as to where the files are at.

	@echo "Moving tools_gli_preview to ../"
	@echo "Moving tools_textured_cube to ../"
	@echo "Moving tools_convertTGAtoGLi to ../"
	@echo "Moving tools_image_manip to ../"
	@echo ""

debug:
	@echo ""
	@echo "/tools_src/Makefile"
	@echo "Building tools_gli_preview.c (DEBUG)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) $(DEBUG) tools_gli_preview.c \
	-o ../tools_gli_preview $(TOOLS_LFLAGS)
	
	@echo "Building tools_textured_cube.c (DEBUG)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) $(DEBUG) tools_textured_cube.c \
	-o ../tools_textured_cube $(TOOLS_LFLAGS)

	@echo "Building tools_convertTGAtoGLi.c (DEBUG)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) $(DEBUG) tools_convertTGAtoGLi.c \
	-o ../tools_convertTGAtoGLi $(TOOLS_LFLAGS) -lTGA

	@echo "Building tools_image_manip.c (DEBUG)"
	@ $(GCC) $(TOOLS_INC) $(TOOLS_LIBS) $(TOOLS_CFLAGS) $(DEBUG) tools_image_manip.c \
	-o ../tools_image_manip $(TOOLS_LFLAGS)

# NOTE: These files are technically already there, but printing this
# should remove any confusion as to where the files are at.
	
	@echo "Moving tools_gli_preview to ../"
	@echo "Moving tools_textured_cube to ../"
	@echo "Moving tools_convertTGAtoGLi to ../"
	@echo "Moving tools_image_manip to ../"
	@echo ""

clean:
	@echo ""
	@echo "/tools_src/Makefile"
	@echo "Removing ../tools_*"
	@ rm ../tools_gli_preview
	@ rm ../tools_textured_cube
	@ rm ../tools_convertTGAtoGLi
	@ rm ../tools_image_manip
	@echo ""

# UPDATE: 9/21/2009
# 	Main development platform is now Ubuntu 9.04. Had to install 'freeglut'
# 	and 'freeglut-dev' from Synaptic. 

all:
	@ cd libGLi_src/ ; make
	@ cd libTGA_src/ ; make
	@ cd tools_src/ ; make
		
debug:
	@ cd libGLi_src/ ; make debug
	@ cd libTGA_src/ ; make debug
	@ cd tools_src/ ; make debug

clean:
	@ cd libGLi_src/ ; make clean
	@ cd libTGA_src/ ; make clean
	@ cd tools_src/ ; make clean
	

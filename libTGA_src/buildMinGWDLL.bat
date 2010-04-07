gcc -c -DBUILD_DLL -I..\include\ -L../lib/ libTGA.c
dllwrap --output-lib=libTGA.a --dllname=TGA.dll --driver-name=gcc -s libTGA.o -lglut32
del libTGA.o
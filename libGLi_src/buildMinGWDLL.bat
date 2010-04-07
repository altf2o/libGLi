; gcc -c -DBUILD_DLL -I..\include\ libGLi.c
; dllwrap --output-lib=libGLi.a --dllname=GLi.dll --driver-name=gcc -s libGLi.o -lglut32 -lopengl32 -lzdll
gcc -I../include/ -shared -o ../bin/libGLi.dll libGLi.c -Wl,--out-implib,../lib/liblibGLi.a -lglut32 -lopengl32 -lglu32 -lz
; del libGLi.o
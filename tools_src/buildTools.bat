gcc -I../include/ -L../lib/ -s -o ../tools_gli_preview.exe tools_gli_preview.c -lGLi -lopengl32 -lglu32 -lglut32 -lzdll
gcc -I../include/ -L../lib/ -s -o ../tools_textured_cube.exe tools_textured_cube.c -lGLi -lopengl32 -lglu32 -lglut32 -lzdll
gcc -I../include/ -L../lib/ -s -o ../tools_convertTGAtoGLi tools_convertTGAtoGLi.c -lGLi -lTGA -lglut32 -lzdll
gcc -I../include/ -L../lib/ -s -o ../tools_image_manip tools_image_manip.c -lGLi -lopengl32 -lglu32 -lglut32 -lzdll
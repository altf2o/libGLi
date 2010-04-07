/*
 * libGLi - A library for loading\saving etc... GLi images
 * Copyright (C) 2005  Frank L. Morales II (altf2o)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * File:
 *  tools_image_manip.c
 *
 * Purpose:
 *  Demonstrates some simple image manipulation techniques.
 *
 * File ID:
 *  $Id: tools_image_manip.c 8 2005-03-05 23:43:06Z altf2o $
 */
#include <GLi.h>

/* Call back and initialization functions */
void Init(void);
void ChangeSize(GLint w, GLint h);
void DisplayScene(void);
void MenuFunc(GLint val);

/* Our right click menu will set this when a new option is chosen.
 * For now we initialize it to a value GREATER than any option.
 */
GLint renderMode = 25;

/* Our global image structure, which will load the image, from the name they
 * pass in on the command line.
 */
gli_image *img;

int
main(int    argc, 
     char **argv)
{   
  if(argc < 2) 
  {
    puts("\nUsage: tools_image_manip <image>  , where <image> is a valid GLi "
         "image");
    return -1;
  }

  /* Load our image so we can get details we need for our Window size etc... */
  img = gli_load(argv[1]);
  if(img == NULL)
    return -1;
    
  printf("img.mBytes   = 0x%02X %c %c %c\n"
         "img.version  = %d\n"
         "img.format   = %s\n"
         "img.width    = %u\n"
         "img.height   = %u\n"
         "img.pxlDepth = %u (bytes)\n"
         "img.imgSize  = %u (raw uncompressed bytes)\n"
         "img.author   = %s\n"
         "img.stamp    = %s\n"
         "img.comp     = %s\n"
         "img.compSize = %u (bytes)\n",
         img->mBytes[0], img->mBytes[1], img->mBytes[2], img->mBytes[3],
         (int)img->version,
         (img->format == GLI_FORMAT_STANDARD) ? "GLI_FORMAT_STANDARD" :
         (img->format == GLI_FORMAT_32BIT)    ? "GLI_FORMAT_32BIT"    : 
                                                "GLI_FORMAT_8BIT",
         img->width,
         img->height,
         img->pxlDepth,
         img->imgSize,
         img->author,
         img->stamp,
         (img->comp == GLI_IMAGE_COMPRESSED) ? "GLI_IMAGE_COMPRESSED" : 
                                               "GLI_IMAGE_NOTCOMPRESSED",
         img->compSize );
            
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(img->width, img->height);
  glutCreateWindow("tools_image_manip - Manipulate a GLi image");
  glutDisplayFunc(DisplayScene);
  glutReshapeFunc(ChangeSize);
    
  Init();
    
  glutCreateMenu(MenuFunc);
  glutAddMenuEntry("Save Image...", 0);
  glutAddMenuEntry("Flip Image...", 1);
  glutAddMenuEntry("Show ALL channels...", 2);
  glutAddMenuEntry("Show RED channel only...", 3);
  glutAddMenuEntry("Show GREEN channel only...", 4);
  glutAddMenuEntry("Show BLUE channel only...", 5);
  glutAddMenuEntry("Reduce all channels by 0.5 (desaturate)...", 6);
  glutAddMenuEntry("Increase all channels by 0.5 (saturate)...", 7);
  glutAddMenuEntry("Invert color...", 8);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
        
  glutMainLoop();

  /* When we're all done we can go ahead and destroy our image 
   *
   * NOTE: Everytime i try and print something after 'glutMainLoop'
   * it fails to print. That to me implies 'glutMainLoop' is likely
   * using the 'exit( ... )' or similar function to immediately
   * exit without returning to this function. If that's the case 
   * this call below to 'gli_destroy' will never run, and you'll 
   * leak the memory the image used. Until i can find another
   * solution (besides loading the image on every pass) this will
   * have to suffice. Consider yourself warned.  FLMII (altf2o)
   */
  gli_destroy(&img);
  return 0;
}


void
Init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
}


void 
ChangeSize(GLint w, 
           GLint h)
{
  glViewport(0, 0, w, h);
    
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    
  gluOrtho2D(0, w, 0, h);
    
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void
DisplayScene(void)
{
  static GLboolean pixelsFlipped = GL_FALSE;
  static GLfloat   saturation    = 1.0;
  GLfloat          colorMap[256];
  GLint            i;
    
  glClear(GL_COLOR_BUFFER_BIT);
        
  /* 
   * Let OpenGL know that when it reads our pixel data from memory it's 
   * TIGHTLY packed. Not packed to it's default of 4 bytes.
   */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
  switch(renderMode) 
  {
    case 1:
      if(pixelsFlipped != GL_TRUE) 
      {
        glPixelZoom(-1.0, -1.0);
        glRasterPos2i(img->width, img->height);
        pixelsFlipped = GL_TRUE;
      }
      else 
      {
        glPixelZoom(1.0, 1.0);
        glRasterPos2i(0, 0);
        pixelsFlipped = GL_FALSE;
      }
      break;
                
    case 2:
      glPixelTransferf(GL_RED_SCALE, 1.0);
      glPixelTransferf(GL_GREEN_SCALE, 1.0);
      glPixelTransferf(GL_BLUE_SCALE, 1.0);
      break;
            
    case 3:
      glPixelTransferf(GL_RED_SCALE, 1.0);
      glPixelTransferf(GL_GREEN_SCALE, 0.0);
      glPixelTransferf(GL_BLUE_SCALE, 0.0);
      break;
            
    case 4:
      glPixelTransferf(GL_RED_SCALE, 0.0);
      glPixelTransferf(GL_GREEN_SCALE, 1.0);
      glPixelTransferf(GL_BLUE_SCALE, 0.0);
      break;
            
    case 5:
      glPixelTransferf(GL_RED_SCALE, 0.0);
      glPixelTransferf(GL_GREEN_SCALE, 0.0);
      glPixelTransferf(GL_BLUE_SCALE, 1.0);
      break;
            
    case 6:
      saturation -= 0.5;
      glPixelTransferf(GL_RED_SCALE, saturation);
      glPixelTransferf(GL_GREEN_SCALE, saturation);
      glPixelTransferf(GL_BLUE_SCALE, saturation);
      break;
            
    case 7:
      saturation += 0.5;
      glPixelTransferf(GL_RED_SCALE, saturation);
      glPixelTransferf(GL_GREEN_SCALE, saturation);
      glPixelTransferf(GL_BLUE_SCALE, saturation);
      break;
            
    case 8:
      colorMap[0] = 1.0f;
      for(i = 1; i < 256; i++)
        colorMap[i] = 1.0f - (1.0f / 255.0f * (GLfloat)i);
                
      glPixelMapfv(GL_PIXEL_MAP_R_TO_R, 255, colorMap);
      glPixelMapfv(GL_PIXEL_MAP_G_TO_G, 255, colorMap);
      glPixelMapfv(GL_PIXEL_MAP_B_TO_B, 255, colorMap);
      glPixelTransferi(GL_MAP_COLOR, GL_TRUE);
      break;
            
    default:
      break;
  }
  glDrawPixels(img->width, 
               img->height, 
               img->format, 
               GL_UNSIGNED_BYTE, 
               img->image);
  glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
  glFlush();
}


void 
MenuFunc(GLint val)
{
  switch(val) 
  {
    case 0:
      if(gli_writeOGLBuffer("screenshot.gli", 
                            "altf2o", 
                            "Unknown",
                            GLI_IMAGE_COMPRESS) == GLI_OK)
        puts("Image: screenshot.gli, written ok.");
      else
        puts("Error writing image: screenshot.gli");
      break;
        
    default:
      renderMode = val;
      break;
  }
  glutPostRedisplay();
}

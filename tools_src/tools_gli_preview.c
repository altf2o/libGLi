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
 *  tools_gli_preview.c
 *
 * Purpose: 
 *  Preview a valid GLi image
 *
 * File ID:
 *  $Id: tools_gli_preview.c 8 2005-03-05 23:43:06Z altf2o $
 */ 
#include <GLi.h>

/* Callbacks and initialization functions */
void Init(void);
void ChangeSize(GLint w, GLint h);
void DisplayScene(void);

/* Image structure that will hold the image, whos name was passed on by the user
 * on the command line.
 */
gli_image * img;

int
main(int     argc, 
     char ** argv)
{   
  if(argc < 2) 
  {
    puts("\nUsage: tools_gli_preview image  , where image is a valid GLi "
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
         "img.pxlDepth = %u ( * 8 = total bpp)\n"
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
  glutCreateWindow("tools_gli_preview - Preview a GLi image");
  glutDisplayFunc(DisplayScene);
  glutReshapeFunc(ChangeSize);
  
  Init();
        
  glutMainLoop();

  /* NOTE:
   *  glutMainLoop() may not be returning to this main function after the
   *  program exits. If it is NOT then this call to 'gli_destroy' will NEVER
   *  be called. Resulting in a memory leak of the image we were just viewing.
   *  I'm working on an alternative, but until then consider yourself
   *  warned. FLMII (altf2o)
   */
  gli_destroy(&img);
    
  return 0;
}


void
Init( void )
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

  glClear(GL_COLOR_BUFFER_BIT);
    
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glRasterPos2i(0, 0);
  glDrawPixels(img->width, 
               img->height, 
               img->format, 
               GL_UNSIGNED_BYTE, 
               img->image);
  glFlush();
}

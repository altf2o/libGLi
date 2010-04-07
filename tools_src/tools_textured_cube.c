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
 *  tools_textured_cube.c
 *
 * Purpose:
 *  Demonstrates texture mapping using our custom GLi images.
 *
 * File ID:
 *  $Id: tools_textured_cube.c 8 2005-03-05 23:43:06Z altf2o $
 */
#include <GLi.h>
#include <stdio.h>
#include <stdlib.h>

/* Global variables to keep track of our cubes "rotation." */
static GLfloat xRot, yRot;

/* Initialization routines, and call back functions */
int Init(void);
void DisplayScene(void);
void ResizeWindow(GLint width, GLint height);
void SpecialKeyFunc(GLint key, GLint x, GLint y);
void TimerFunc(GLint val);


int
main(int    argc, 
     char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutCreateWindow("tools_textured_cube - Demonstrating how to use libGLi for"
                   " OpenGL texture mapping...");
  glutReshapeFunc(ResizeWindow);
  glutDisplayFunc(DisplayScene);
  glutSpecialFunc(SpecialKeyFunc);
  glutTimerFunc(50, TimerFunc, 0);
    
  if(Init())
    exit(EXIT_FAILURE);

  glutMainLoop();
    
  return 0;
}


int
Init(void)
{
  gli_image *img;
    
  GLfloat ambient_light[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat diffuse_light[] = { 0.7, 0.7, 0.7, 1.0 };
  GLfloat specular_light[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat spec_ref[] = { 1.0, 1.0, 1.0, 1.0 };

  /* Turn on a few things */
  glClearColor(0.3, 0.3, 0.3, 0.3);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
    
  /* Set up all our lighting information */
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
  glMaterialfv(GL_LIGHT0, GL_SPECULAR, spec_ref);
  glMateriali(GL_LIGHT0, GL_SHININESS, 120);
    
  /* Hey who turned on the lights ? */
  glEnable(GL_LIGHT0);
    
  /* Load our wood texture */
#if defined (WIN32)
  if((img = gli_load("..\\samples\\wood.gli")) == NULL) 
  {
#else /* defined (__FreeBSD__) */
  if((img = gli_load("samples//wood.gli")) == NULL) 
  {
#endif
    puts("Error loading ..\\samples\\wood.gli, exiting");
    return -1;
  }
    
  /* Got our texture ok. Load it into our current texture "state" */
  glTexImage2D(GL_TEXTURE_2D, 
               0, 
               img->format, 
               img->width, 
               img->height, 
               0,
               img->format, 
               GL_UNSIGNED_BYTE, 
               img->image);
    
  /* Since OpenGL makes a copy of it, we no longer need this. Our 
   * texture will be with us for the duration of our program.
   */                             
  gli_destroy(&img);
    
  /* Set our texture filtering to LINEAR (pretty decent quality). */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
  /* Tell OpenGL to combign whats currently in the color buffer w/ each
   * texel from our texture.
   */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
    
  /* Cull (don't render) faces when you can't see them. */
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
    
  printf("Commands:\n"
         "\n"
         "    F12 - To take a screenshot of the current OpenGL window.\n"
         "          (NOTE: This only writes \"screenshot.gli\" consecutive\n"
         "           F12 presses overwrite that file each time.\n");
  return 0;
}


void
DisplayScene(void)
{
  /* Clear our buffers each time we render */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
  if(xRot == 360)
    xRot = yRot = 0.0;
  else 
  {
    xRot += 1.5;
    yRot += 1.5;
  }
    
  /* Rotate our coordinate system, then plot our texture mapped cube */
  glPushMatrix();
    glRotatef(xRot, 1.0, 0.0, 0.0);
    glRotatef(yRot, 0.0, 1.0, 0.0);
    
    
    glColor3f(0.5, 0.5, 0.0);
    glBegin(GL_QUADS);
      /* Front face */
      glNormal3f(0.0, 0.0, 1.0);
      glTexCoord2f(0.0, 0.0);
      glVertex3f(-20, -20, 0);
            
      glTexCoord2f(1.0, 0.0);
      glVertex3f(20, -20, 0);
            
      glTexCoord2f(1.0, 1.0);
      glVertex3f(20, 20, 0);
            
      glTexCoord2f(0.0, 1.0);
      glVertex3f(-20, 20, 0);
            
      /* Right face */
      glNormal3f(1.0, 0.0, 0.0);
      glTexCoord2f(0.0, 0.0);
      glVertex3f(20, -20, 0);
            
      glTexCoord2f(1.0, 0.0);
      glVertex3f(20, -20, -40);
            
      glTexCoord2f(1.0, 1.0);
      glVertex3f(20, 20, -40);
            
      glTexCoord2f(0.0, 1.0);
      glVertex3f(20, 20, 0);
            
      /* Rear face */
      glNormal3f(0.0, 0.0, -1.0);
      glTexCoord2f(0.0, 0.0);
      glVertex3f(20, -20, -40);
            
      glTexCoord2f(1.0, 0.0);
      glVertex3f(-20, -20, -40);
            
      glTexCoord2f(1.0, 1.0);
      glVertex3f(-20, 20, -40);
            
      glTexCoord2f(0.0, 1.0);
      glVertex3f(20, 20, -40);
            
      /* Left face */
      glNormal3f(-1.0, 0.0, 0.0);
      glTexCoord2f(0.0, 0.0);
      glVertex3f(-20, -20, -40);
            
      glTexCoord2f(1.0, 0.0);
      glVertex3f(-20, -20, 0);
            
      glTexCoord2f(1.0, 1.0);
      glVertex3f(-20, 20, 0);
            
      glTexCoord2f(0.0, 1.0);
      glVertex3f(-20, 20, -40);
            
      /* Top face */
      glNormal3f(0.0, 1.0, 0.0);
      glTexCoord2f(0.0, 0.0);
      glVertex3f(-20, 20, 0);
            
      glTexCoord2f(1.0, 0.0);
      glVertex3f(20, 20, 0);
            
      glTexCoord2f(1.0, 1.0);
      glVertex3f(20, 20, -40);
            
      glTexCoord2f(0.0, 1.0);
      glVertex3f(-20, 20, -40);
            
      /* Bottom face */
      glNormal3f(0.0, -1.0, 0.0);
      glTexCoord2f(0.0, 0.0);
      glVertex3f(-20, -20, 0);
            
      glTexCoord2f(0.0, 1.0);
      glVertex3f(-20, -20, -40); 
            
      glTexCoord2f(1.0, 1.0);
      glVertex3f(20, -20, -40);
            
      glTexCoord2f(1.0, 0.0);
      glVertex3f(20, -20, 0); 
    glEnd();
  glPopMatrix();
    
  glutSwapBuffers();
  
  glutTimerFunc(50, TimerFunc, 0);
}


void
ResizeWindow(GLint width, 
             GLint height)
{
  glViewport(0, 0, width, height);
    
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    
  gluPerspective(60.0f, (GLfloat)width/(GLfloat)height, 25.0, 200.0);
  glTranslatef(0.0, 0.0, -75.0);
  gluLookAt(1.0, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void
SpecialKeyFunc(GLint key, 
               GLint x, 
               GLint y)
{
  switch(key) 
  {
    case GLUT_KEY_F12:
      if(gli_writeOGLBuffer("screenshot.gli", 
                            "Frank Morales",
                            "Unknown", 
                            GLI_IMAGE_COMPRESS) == GLI_OK)
        puts("screenshot.gli written ok");
      else
        puts("Error writing screenshot.gli");
      break;

    default:
      break;
  }
}


void
TimerFunc(GLint val)
{
  glutPostRedisplay();
}

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
 *  libGLi.h
 *
 * Purpose:
 *  This defines the public API for our libGLi library.
 *
 * File ID:
 *  $Id: GLi.h 8 2005-03-05 23:43:06Z altf2o $
 */     

#ifndef GLI_H
#define GLI_H

/* If we're on Winders, we can make this library a DLL */
#if defined ( WIN32 )
# define DLLEXPORT __declspec( dllexport )
# include <gl/glut.h>
# include <gl/glext.h>
#elif defined ( __FreeBSD__ )
# define DLLEXPORT
# include <GL/glut.h>
# include <GL/glext.h>
#else
# define DLLEXPORT /* This is for Linux, what should we test for? */
# include <GL/glut.h>
# include <GL/glext.h>
#endif

#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This will convert our pixel data from: BGR form, to RGB. */
#define CONVERTBGRtoRGB( img )            \
  unsigned char bgr_byte;                 \
  unsigned int  u;                        \
  for( u = 0; u < img->imgSize; u += 3 )  \
  {                                       \
    bgr_byte = img->image[u];             \
    img->image[u] = img->image[u + 2];    \
    img->image[u + 2] = bgr_byte;         \
  }                                       
      
/* This will convert our pixel data from: BGRA form, to RGBA. */
#define CONVERTBGRAtoRGBA( img )          \
  unsigned char bgr_byte;                 \
  unsigned int  u;                        \
  for( u = 0; u < img->imgSize; u += 4 )  \
  {                                       \
    bgr_byte = img->image[u];             \
    img->image[u] = img->image[u + 2];    \
    img->image[u + 2] = bgr_byte;         \
  }

/* Size of our header MINUS the 4 byte pointer for 'image' */
#define GLI_HEADER_SIZE 79

/* Right now this MUST be 1 */
#define GLI_CURRENT_VERSION 1

/* All of our images are stored using: RGB\RGBA format. 
 * Or single byte per pixel luminace. 
 */
#define GLI_FORMAT_STANDARD GL_RGB
#define GLI_FORMAT_32BIT    GL_RGBA
#define GLI_FORMAT_8BIT     GL_LUMINANCE

/* Just some meaningful return values for our functions to use */
#define GLI_OK    0
#define GLI_ERROR !GLI_OK

/* These let us know if we should compress the image, and if it is 
 * /already/ compressed etc... 
 */
#define GLI_IMAGE_COMPRESS      0
#define GLI_IMAGE_DONTCOMPRESS  1
#define GLI_IMAGE_COMPRESSED    2
#define GLI_IMAGE_NOTCOMPRESSED 3

/* Every valid GLi image MUST contain these as it's first 4 bytes */
static unsigned const char MAGIC_BYTES[] = { 0x7F, 'G', 'L', 'i' };

/* Your compiler must support this directive for this library to properly
 * work. If it does not, please change this #pragma to something your
 * compiler understands so it keeps our structure "tightly" packed.
 */
#pragma pack(1)
typedef struct _gli_image 
{
  unsigned char  mBytes[4];
  unsigned char  version;
  unsigned int   format;
  unsigned int   width;
  unsigned int   height;
  unsigned char  pxlDepth;
  unsigned int   imgSize;
  char           author[24];
  char           stamp[28];
  unsigned char  comp;
  unsigned int   compSize;
  char          *image;
} gli_image;
#pragma pack(4)

/* Use extern "C" here so we can call these from C++ programs */
#ifdef __cplusplus
extern "C" {
#endif

/* Public API function prototypes */
DLLEXPORT gli_image *gli_load          (const char *in_file);
DLLEXPORT int        gli_write         (const gli_image *img, 
                                        const char      *out_file);
DLLEXPORT void       gli_destroy       (gli_image **img);

DLLEXPORT int        gli_compress      (char              *dest_buff,
                                        unsigned long int *dest_len,
                                        const char        *src_buff,
                                        unsigned long int  src_len);

DLLEXPORT int        gli_uncompress    (char              *dest_buff,
                                        unsigned long int *dest_len,
                                        const char        *src_buff,
                                        unsigned long int  src_len);
DLLEXPORT int        gli_writeOGLBuffer(const char    *out_file, 
                                        const char    *author, 
                                        const char    *stamp,
                                        unsigned char  compress);
#ifdef __cplusplus
}
#endif

#endif /* not GLI_H */

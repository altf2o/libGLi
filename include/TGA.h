/*
 * libTGA - Minimalist library to load a select few targa images
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
 *  libTGA.h
 *
 * Purpose:
 *  This defines the public API for our libTGA library
 *
 * File ID:
 *  $Id: TGA.h 4 2005-03-04 10:36:51Z altf2o $
 */
#ifndef TGA_H
#define TGA_H

#if defined ( WIN32 )
# define DLLEXPORT __declspec( dllexport )
# include <windows.h>
# include <gl/glut.h>
# include <gl/glext.h>
#elif defined ( __FreeBSD__ )
# define DLLEXPORT
# include <GL/glut.h>
# include <GL/glext.h>
# include <sys/types.h>
# include <sys/stat.h>
#endif

#include <GLi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Keep ALL of these structures tightly packed so we can
 * safely read the entire thing from the file in a SINGLE
 * fread statement.
 */
#pragma pack(1)
typedef struct _tga_image 
{
  /* Header */
  unsigned char   idLength;
  unsigned char   colorMapType;
  unsigned char   imageType;
    
  /* Color map Specification */
  unsigned short  firstEntryIndex;
  unsigned short  colorMapLength;
  unsigned char   cMapEntrySize;
   
  /* Image specification */
  unsigned short  xorigin;
  unsigned short  yorigin;
  unsigned short  width;
  unsigned short  height;
  unsigned char   pxlDepth;
  unsigned char   imgDescriptor;
    
  /* Various helpful variables */
  unsigned int    fileSize;
  unsigned int    cMapSize;
  unsigned int    imgOffset;
  unsigned int    imgSize;
  unsigned char  *image;
    
  /* Footer */
  unsigned int    extOffset;
  unsigned int    devDirOffset;
    
  /* This should hold:  TRUEVISION-XFILE.0  where 0 is a binary zero */
  unsigned char   signature[17];
} tga_image;
#pragma pack(4)
/* End of tightly packed structures */

#define TGA_TYPE_TRUE_COLOR 2

DLLEXPORT tga_image *tga_load( const char *in_file );
DLLEXPORT void       tga_destroy( tga_image **my_tga );

#endif /* not TGA_H */

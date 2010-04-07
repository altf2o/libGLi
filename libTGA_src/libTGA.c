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
 *  libTGA.c
 *
 * Purpose:
 *  This implements the public API we use in libTGA.
 *
 * File ID:
 *  $Id: libTGA.c 8 2005-03-05 23:43:06Z altf2o $
 */ 
#include <TGA.h>


/*
 * Arguments:
 *  in_file - A valid C-Strings containing the name of the TGA image to load.
 *
 * Purpose:
 *  This function loads up a TGA(TARGA) image, and stores it's information
 *  into a dynamically allocated structure (img). Which will get returned.
 *
 * Returns:
 *   img - If everything went ok.
 *  NULL - On error.
 */
DLLEXPORT tga_image *
tga_load(const char *in_file)
{
  tga_image *img;
  FILE      *fin;
    
  if(!in_file)
    return NULL;
    
  img = malloc(sizeof(*img));
  if(img == NULL)
    return NULL;
        
  memset(img, 0, sizeof(*img));
      
  fin = fopen(in_file, "rb");  
  if(fin == NULL) 
  {
    free(img);
    return NULL;
  }
    
  if(fread(img, 18, 1, fin) < 1)
    goto error;
        
  /* We only understand True Color non RLE compressed targa images */
  if(img->imageType != TGA_TYPE_TRUE_COLOR)
    goto error;
    
  img->pxlDepth /= 8;
    
  /* We only understand 8\24\32bit images. */
  if(img->pxlDepth != 3 && img->pxlDepth != 4 && img->pxlDepth != 1)
    goto error;
        
  img->imgSize = img->width * img->height * img->pxlDepth;
    
  img->image = malloc(img->imgSize * sizeof(*img->image));
  if(img->image == NULL)
    goto error;
    
  /* A platform specific, but reliable way to get our file size */
#if defined (WIN32)
  HANDLE targa;
    
  targa = CreateFile(in_file, 
                     GENERIC_READ, 
                     FILE_SHARE_READ, 
                     NULL,
                     OPEN_EXISTING, 
                     FILE_ATTRIBUTE_NORMAL, 
                     NULL);              
  if(targa == INVALID_HANDLE_VALUE)
    goto error;
    
  img->fileSize = GetFileSize(targa, NULL);
  if(img->fileSize == 0xFFFFFFFF)
    goto error;

  CloseHandle( targa );
#elif defined (__FreeBSD__)
  struct stat sb;
    
  if(stat(in_file, &sb))
    goto error;
        
  img->fileSize = sb.st_size;
#endif

  if(fseek(fin, img->fileSize - 26, SEEK_SET))
    goto error;
  if(fread( &img->extOffset, 4, 1, fin ) < 1)
    goto error;      
  if(fread( &img->devDirOffset, 4, 1, fin ) < 1)
    goto error;    
  if(fread( &img->signature, 17, 1, fin ) < 1) 
    goto error;
        
  img->signature[16] = '\0';
    
  if(!memcmp(img->signature, "TRUEVISION-XFILE", 15)) 
  {
    if(img->extOffset || img->devDirOffset)
      goto error;
  }
  else 
  {
    img->extOffset = 0;
    img->devDirOffset = 0;
    memset(&img->signature, 0, 17);
  }
    
  /* Start off seeking passed our header for sure. */
  img->imgOffset = 18;
    
  /* If we have an Image ID section we want to seek passed it. */
  if(img->idLength)
    img->imgOffset += img->idLength;
        
  /* If we have a color map section, we want to seek passed it */
  if(img->colorMapType)
    img->imgOffset += (img->colorMapLength * img->cMapEntrySize);
    
  if(fseek(fin, img->imgOffset, SEEK_SET))
    goto error;
    
  if(fread(img->image, img->imgSize, 1, fin) < 1)
    goto error;
    
  fclose(fin);
  return img;

error:
  tga_destroy(&img);
  fclose(fin);
  return NULL;
}


/* 
 * Arguments:
 *  img - Address of a dynamically allocated 'tga_image' structure. Like one 
 *        returned from 'tga_load' tga_image * img; 
 *        img = tga_load("image.tga"); tga_destroy( &img );
 *
 * Purpose:
 *  This is a simple function to clean up a currently loaded TARGA image.
 *
 * Returns:
 *  Nothing.
 */
DLLEXPORT void 
tga_destroy(tga_image **img)
{
  if(!img)
    return ;
        
  if((*img)->image) free((*img)->image);
  free((*img));
  *img = NULL; 
}

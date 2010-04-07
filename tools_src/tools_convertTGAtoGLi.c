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
 *  tools_convertTGAtoGLi.c
 *
 * Purpose: 
 *  Convert your basic TARGA images into our own GLi format
 *
 * File ID:
 *  $Id: tools_convertTGAtoGLi.c 8 2005-03-05 23:43:06Z altf2o $
 */ 
#include <TGA.h>

int
main(int    argc, 
     char **argv)
{
  unsigned long int  comp_size = 0;
  char              *comp_data = NULL;
  char              *ext       = NULL;
  char               name[50]  = {0};
  tga_image         *img       = NULL;
  gli_image          glimg;
  FILE              *fout      = NULL;
    
  if(argc < 4) 
  {
    puts("Usage: convertTGAtoGLi <image>.tga <author> <date\\time stamp>");
    exit(EXIT_FAILURE );
  }
    
  /* Prepare a name we'll use as our output file. */
  strncpy(name, argv[1], 49);
  if(strlen(name) <= 44) 
  {
    ext = strrchr(name, '.');
    if(ext != NULL) 
    {
      ext++;
      strncpy(ext, "gli", 4);
    }
    else
      strncpy((name + strlen(name) - 4), ".gli", 5);
  }
  else
    strncpy((name + 44), ".gli", 5);

  img = tga_load(argv[1]);
  if(img == NULL) 
  {
    printf("Error loading: %s for reading, exiting\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  
  fout = fopen(name, "wb");  
  if(fout == NULL) 
  {
    printf("Error opening %s for writing, exiting\n", name);
    exit(EXIT_FAILURE);
  }
    
  /* Zero out our entire image structure, setting ALL fields to a known state */
  memset(&glimg, 0, sizeof(glimg));
    
  if(img->pxlDepth == 3) 
  {
    CONVERTBGRtoRGB(img)
  }
  else if(img->pxlDepth == 4) 
  {
    CONVERTBGRAtoRGBA(img)
  }
    
  /* Set up our structure */
  memcpy(glimg.mBytes, MAGIC_BYTES, 4);
  glimg.version = GLI_CURRENT_VERSION;
  glimg.format = (img->pxlDepth == 3) ? GLI_FORMAT_STANDARD : 
                 (img->pxlDepth == 4) ? GLI_FORMAT_32BIT    : GLI_FORMAT_8BIT;
  glimg.width = img->width;
  glimg.height = img->height;
  glimg.pxlDepth = img->pxlDepth;
  glimg.imgSize = img->imgSize;
  strncpy( glimg.author, argv[2], 23 );
  strncpy( glimg.stamp, argv[3], 27 );
  glimg.comp = GLI_IMAGE_COMPRESSED;
    
  /* Lets grab a buffer equal to the raw pixel datas size. That should be 
   * sufficiant to hold the compressed data.
   */
  comp_data = malloc(img->imgSize);
  if(comp_data == NULL)
    goto error;
    
  /* Here we gotta remember to let gli_compress know how big our destination 
   * buffer is 
   */
  comp_size = img->imgSize;
    
  /* Now lets try compressing our actual pixel data */
  if(gli_compress(comp_data, 
                  &comp_size, 
                  (const char *)img->image, 
                  img->imgSize) != GLI_OK)
    goto error;
    
  /* comp_size holds the actual size our data was compressed to (in bytes). 
   * Store that, and make glimg.image point to our comp_data memory (compressed 
   * data). Be sure we set comp_data to NULL since we'll free that memory later 
   * through glimg.image.
   */
  glimg.compSize = comp_size;
  glimg.image = comp_data;
  comp_data = NULL;

  /* Write our actual converted GLi image to disk. */
  if(fwrite(&glimg, GLI_HEADER_SIZE, 1, fout) < 1)
    goto error;

  if(fwrite(glimg.image, glimg.compSize, 1, fout) < 1)
    goto error;
    
  printf("File: %s , written ok...\n", name);
  tga_destroy(&img);
  free(glimg.image);
  fclose(fout);
  return 0;

error:
  printf("ERROR writing file: %s \n", name);
  tga_destroy(&img);
  if(comp_data)   free(comp_data);
  if(glimg.image) free(glimg.image);
  fclose( fout );
  return -1;
}

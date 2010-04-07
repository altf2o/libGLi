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
 *  libGLi.c
 *
 * Purpose: 
 *  This is the implementation for our libraries exported functions.
 *
 * File ID:
 *  $Id: libGLi.c 8 2005-03-05 23:43:06Z altf2o $
 */ 
#include <GLi.h>

/*
 * Arguments:
 *  in_file - A valid C-String containing the name of the GLi image to load.
 *
 * Purpose:
 *  This function will dynamically allocate a 'gli_image' structure, filling
 *  it in with the image, whos name is passed in (in_file). If the image is 
 *  compressed this function will uncompress the pixel data. Finally it 
 *  stores the raw pixel data in the 'gli_image' structure' 'image' variable.
 *
 * Returns:
 *   img - A valid 'gli_image' structure, completely filled in.
 *  NULL - On error.
 */
DLLEXPORT gli_image * 
gli_load(const char *in_file)
{
  unsigned long int  uncompSize  = 0;
  gli_image         *img         = NULL;
  GLubyte           *comp_data   = NULL;
  FILE              *fin;
    
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
  
  /* First lets snag our header */
  if(fread(img, GLI_HEADER_SIZE, 1, fin) < 1)
    goto error;

  /* Wrong magic byte or version, it's an error */
  if(memcmp(img->mBytes, MAGIC_BYTES, 4) || 
     img->version != GLI_CURRENT_VERSION)
    goto error;
  
  /* The only images we currently support are 8\24\32bits per pixel. */
  if(img->format != GLI_FORMAT_STANDARD &&
     img->format != GLI_FORMAT_32BIT    &&
     img->format != GLI_FORMAT_8BIT)
    goto error;
        
  /* No valid image size, that's an error */
  if(!img->imgSize)
    goto error;
  
  uncompSize = img->imgSize;
  
  /* Allocate the space for our uncompressed raw bitmap data */
  img->image = malloc(img->imgSize);
  if(img->image == NULL)
    goto error;
      
  /* Is this image compressed ? */
  if(img->comp == GLI_IMAGE_COMPRESSED) 
  {    
    /* There better be a valid size for the compressed data */
    if(img->compSize) 
    {
      comp_data = malloc(img->compSize * sizeof(*comp_data));
      if(comp_data != NULL) 
      {
        /* Got our memory, lets read in our compressed image data */
        if(fread(comp_data, img->compSize, 1, fin) == 1)
        {
          if(gli_uncompress(img->image, 
                            &uncompSize, 
                            comp_data, 
                            img->compSize) == GLI_OK) 
          {
            /* Ok no error in uncompressing the data. Did it unpack to the 
             * same size ? If so our raw uncompressed bitmap data is now being
             * stored in img->image, we no longer need "comp_data", free it.
             * If not, there was an error in the uncompression. 
             */
            if(uncompSize == img->imgSize)
              free(comp_data);
            else
              goto error;
          }
          else
            /* Error in gli_uncompress */
            goto error;
        }
        else
          /* Error in fread */
          goto error;
      }
      else
        /* Error mallocing comp_data memory */
        goto error;
    }
    else
      /* No valid value in compSize */
      goto error;
  }
  else 
  {
    /* Uncompressed image */
    if(fread(img->image, img->imgSize, 1, fin) < 1)
      goto error;
  }
  
  fclose(fin);
  return img;
  
error:
  if(comp_data)  free(comp_data);
  if(img->image) free(img->image);
  free(img);
  fclose(fin);
  return NULL;
}

    
/*
 * Arguments:
 *       img - A pointer to a valid, and filled in 'gli_image' structure.
 *  out_file - A valid C-String specifying the file to write to.
 *
 * Purpose:
 *  This function writes our passed in 'gli_image' structure (img) to the
 *  file specified by 'out_file'. This function expects 'img' to be 
 *  completely filled in, compression and all.
 *
 * Returns:
 *     GLI_OK - If everything went ok.
 *  GLI_ERROR - On error.
 */
DLLEXPORT int 
gli_write(const gli_image *img, 
          const char      *out_file)
{
  unsigned int  imgSize;
  FILE         *fout;
    
  if(!img || !out_file || !img->image)
    return GLI_ERROR;
      
  /* Are we compressed ? If so we better have a valid 'compSize' */
  if(img->comp == GLI_IMAGE_COMPRESSED) 
  {
    /* Anything non zero we'll consider as being valid */
    if(img->compSize)
      imgSize = img->compSize;
    else
      return GLI_ERROR;
  }
  else 
  {
    /* Not compressed, we better have valid image size then */
    if(img->imgSize)
      imgSize = img->imgSize;
    else
      return GLI_ERROR;
  }
      
  fout = fopen(out_file, "wb");
  if(fout == NULL)
    return GLI_ERROR;

  if(fwrite(img, GLI_HEADER_SIZE, 1, fout) < 1) 
  {
    fclose(fout);
    return GLI_ERROR;
  }
  
  if(fwrite(img->image, imgSize, 1, fout) < 1) 
  {
    fclose(fout);
    return GLI_ERROR;
  }
  
  fclose(fout);
  return GLI_OK;
}


/* 
 * Arguments:
 *  img - This should be the 'address' of a valid dynamically allocated
 *        'gli_image' structure. I.e.: 
 *        gli_image * img; img = gli_load("image.gli"); gli_destroy( &img );
 *                         
 * Purpose:
 *  This function destroys a dynamically allocated 'gli_image' structure,
 *  like the one returned from 'gli_load'.
 *
 * Returns:
 *   Nothing.
 */
DLLEXPORT void 
gli_destroy(gli_image **img)
{
  if(!img)
    return ;
      
  free((*img)->image);
  free((*img));
  *img = NULL;
}


/*
 * Arguments:
 *  dest_buff - A pointer to a destination buffer large enough to hold the
 *              compressed data.
 *  dest_len  - The length of our destination buffer.
 *  src_buff  - A pointer to a source buffer that contains our raw pixel data.
 *  src_len   - The length of our source buffer.
 *
 * Purpose:
 *  This function is simply a wrapper to the zlib function call, "compress2",
 *  which uses a default compression of Z_BEST_COMPRESSION. If compress2
 *  works ok we map it's return value to our own GLI_OK, otherwise we map 
 *  it to GLI_ERROR.
 *
 * Returns:
 *     GLI_OK - If everything went ok.
 *  GLI_ERROR - On error.
 */
DLLEXPORT int
gli_compress(char              *dest_buff, 
             unsigned long int *dest_len,
             const char        *src_buff, 
             unsigned long int  src_len)
{
  return (compress2(dest_buff, 
                    dest_len, 
                    src_buff, 
                    src_len, 
                    Z_BEST_COMPRESSION) == Z_OK) ? GLI_OK : GLI_ERROR;
}
                              

/* 
 * Arguments:
 *  dest_buff - A pointer to a destination buffer large enough to hold our
 *              uncompressed data.
 *  dest_len  - The length of our destination buffer.
 *  src_buff  - A pointer to our source buffer that contains our compressed
 *              pixel data.
 *  src_len   - The length of our source buffer.
 *
 * Purpose:
 *  This function is simply a wrapper to the zlib function call, "uncompress"
 *  which uncompresses our source buffer, and stores it in our destination
 *  buffer. Again we map the return of uncompress to a value of our own.
 *
 * Returns:
 *     GLI_OK - If everything went ok.
 *  GLI_ERROR - On error.
 */
DLLEXPORT int  
gli_uncompress(char              *dest_buff, 
               unsigned long int *dest_len,
               const char        *src_buff, 
               unsigned long int  src_len )
{
  return (uncompress(dest_buff, 
                     dest_len, 
                     src_buff, 
                     src_len) == Z_OK) ? GLI_OK : GLI_ERROR;
}

                                                                
/*
 * Arguments:
 *  out_file - This should be a valid C-String containing the name of the 
 *             file to write to.
 *  author   - This should be a valid C-string (23 char max) containing 
 *             the author's name.
 *  stamp    - This should be a valid C-string (27 char max) containing a
 *             data\time stamp.
 *  compress - This should be GLI_IMAGE_COMPRESS if you want to use 
 *             compression. Otherwise you should use: GLI_IMAGE_DONTCOMPRESS.
 *             All values other than: GLI_IMAGE_COMPRESS are assumed to mean
 *             do NOT use compression.
 *
 * Purpose:
 *  This function is specifically for writing what's currently in your OpenGL
 *  windows front buffer (a screenshot if you will). It will determin the
 *  current bits per pixel of your current window, and fill in a valid 
 *  'gli_image' structure. If compression is wanted it does all that as well.
 *  Then it simply passes the filled in 'gli_image' structure, plus the file
 *  you want to write to (out_file) to 'gli_write' which writes the actual
 *  image to disk.
 *
 * Returns:
 *     GLI_OK - If everything went ok.
 *  GLI_ERROR - On error.
 */
DLLEXPORT int 
gli_writeOGLBuffer(const char    *out_file, 
                   const char    *author, 
                   const char    *stamp, 
                   unsigned char  compress)
{
  unsigned long int  comp_size;
  gli_image          img;
  GLubyte           *comp_data;
  GLenum             lastBuff;
  GLint              viewport[4];
  GLint              rBits;
  GLint              gBits;
  GLint              bBits;
  GLint              aBits;

  memset(&img, 0, sizeof(img));
  
  /* Compute how many bits per pixel we're working with */
  glGetIntegerv(GL_RED_BITS, &rBits);
  glGetIntegerv(GL_GREEN_BITS, &gBits);
  glGetIntegerv(GL_BLUE_BITS, &bBits);
  glGetIntegerv(GL_ALPHA_BITS, &aBits);
          
  img.pxlDepth = (rBits + gBits + bBits + aBits) / 8;
                
  /* Right now we're only supporting 8\24\32bits per pixel. */
  if(img.pxlDepth != 3 && img.pxlDepth != 4 && img.pxlDepth != 1)
    return GLI_ERROR;
                
  /* Assign our images format */
  img.format = (img.pxlDepth == 3) ? GLI_FORMAT_STANDARD : 
               (img.pxlDepth == 4) ? GLI_FORMAT_32BIT    : 
               GLI_FORMAT_8BIT;
                
  /* Now we see how big our current window is. viewport[2] contains the 
   * width while viewport[3] contains the height. 
   */        
  glGetIntegerv(GL_VIEWPORT, viewport);
  
  img.imgSize = viewport[2] * viewport[3] * img.pxlDepth;
  
  img.image = malloc(img.imgSize * sizeof(*img.image));
  if(img.image == NULL) 
    return GLI_ERROR;
  
  /* Here we tell OpenGL to keep things tightly packed when we read from the 
   * color buffer and store to memory.
   */
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

  /* We want to preserve the current buffer, just incase the current one
   * isn't GL_FRONT 
   */
  glGetIntegerv(GL_READ_BUFFER, &lastBuff);
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 
               0, 
               viewport[2], 
               viewport[3], 
               img.format, 
               GL_UNSIGNED_BYTE, 
               img.image);
  glReadBuffer(lastBuff);
  
  /* Here we setup the rest of our structures header */
  memcpy(img.mBytes, MAGIC_BYTES, 4);
  img.version = GLI_CURRENT_VERSION;
  img.width = viewport[2];
  img.height = viewport[3];
  strncpy(img.author, author, 23);
  strncpy(img.stamp, stamp, 27);
  
  if(compress == GLI_IMAGE_COMPRESS) 
  {
    /* Lets get a second buffer for compressing the data */
    comp_data = malloc(img.imgSize * sizeof(*comp_data));
    if(comp_data == NULL) 
    {
      free(img.image);
      return GLI_ERROR;
    }
    else 
    {
      comp_size = img.imgSize;
      if(gli_compress(comp_data, &comp_size, img.image, img.imgSize) == GLI_OK)
      {
        /* No longer need our raw bitmap data, free it */
        free(img.image);

        /* Set our image data to point to our compressed data buffer */
        img.image = comp_data;
        comp_data = NULL;
        
        img.comp = GLI_IMAGE_COMPRESSED;
        img.compSize = comp_size;
      }
      else 
      {
        /* Error compressing. Simply store it as an uncompressed image */
        img.comp = GLI_IMAGE_NOTCOMPRESSED;
        img.compSize = 0;
        free(comp_data);
      }
    }    
  }
  else 
  { 
    /* Passed in argument "compress" was NOT: GLI_IMAGE_COMPRESS */  
    img.comp = GLI_IMAGE_NOTCOMPRESSED;
    img.compSize = 0;
  }
  
  /* Now we write our actual GLi image. */
  if(gli_write(&img, out_file) != GLI_OK) 
  {
    free(img.image);
    return GLI_ERROR;
  }
  
  /* Everything went well. The only thing that's dynamically allocated
   * is this. 
   */
  free(img.image);
  return GLI_OK;
}

[NOTE: Please see additional notes at the bottom]

[ COPYRIGHT NOTICES ]
    OpenGL Image library v1.0
    
    GLi and the GLi specification are (C) Frank L. Morales II (altf2o)
    Source code for libGLi (C) Frank L. Morales II (altf2o)
    
    TGA, TARGA, and the TGA specification are (C) Truevision.
    Source code for libTGA (C) Frank L. Morales II (altf2o)
    
    This code makes use of the zlib lossless compression library.
    Information on the zlib library and it's author's can be 
    found at: http://www.gzip.org/zlib/
    
    The OpenGL name and API is copyright it's respective owners.
    Please visit:  http://www.opengl.org/ for more information.
    
    This library targets the OpenGL API but is in no way associated with
    OpenGL or the OpenGL ARB (Architecture Review Board). This is a 100% 3rd
    party and independantly developed image file format and library. I make
    no claims that myself, the name GLi, the GLi specification and libGLi are 
    in any way associated with OpenGL or the OpenGL ARB.

    
[ GOALS OF THIS LIBRARY ]

    The goal was to create an image format that could easily be used in 
    OpenGL texture mapping. I wanted a library that could compress using
    lossless compression techniques (by the well established zlib library)
    as well as have a VERY easy to understand and parse structure. All 
    while keeping the sections that were important to a modern day image
    file format. We also needed a file format where no section was optional.
    This would reduce the robustness of the file format in comparison to 
    some others. However it would greatly reduce the complexity of parsing the 
    image, which would lead to improved performance and smaller library code.
    The result is the GLi specification. The goals and how i feel i 
    accomplished them are as follows:
    
    1) We needed to be able to quickly and easily determine if the current
    file is a valid GLi image. Therefor i took a note from the 'ELF' 
    executable file format and used a similar 4 byte field. The first 
    variable in our image header is: 
        mBytes (4 bytes). 
    Our images (if they're valid) start w/ the first 4 bytes being:  
        0x7F, 'G', 'L', 'i' . 
    This allows us to quickly determine if the file is valid or not and
    act accordingly. To make it easier we declare an array (in GLi.h)
    which contains these 4 bytes called, MAGIC_BYTES. Using any type of
    memory compare function we can quickly and easily determine if the
    current file is valid.

    2) We needed a way to quickly and most importantly RELIABLY establish the
    EXACT version of the image format used to write the image we're trying to
    work with. We introduce the next variable in our header: 
        version (1 byte). 
    This will contain the version used to write the GLi image. To make it
    easier, this value can be set with, and compared against the #define
    from GLi.h:
        GLI_CURRENT_VERSION
    As of this writing the ONLY possible value is 1.

    3) Next we needed a way to specify a meaningful value to tell OpenGL which
    format our pixel data is in. We introduce the next variable in our header:
        format (4 bytes). 
    To keep things simple we addressed the problem of reversed pixel data, i.e.
    BGR form as apposed to the standard RGB. To make it easier our format ONLY
    supports storing pixel data in the RGB format. As of version 1.0 we support
    8\24\32 bits per pixel. We offer 2 macros in GLi.h the first being:
        CONVERTBGRtoRGB 
    which expects a pointer to a valid 'gli_image' structure. And:
        CONVERTBGRAtoRGBA
    which also expects a pointer to a valid 'gli_image' structure. Both of which
    simply reverse the image data's pixel order.

    4) The next 3 fields are the images "width":
        width (4 bytes)
    "height":
        height (4 bytes) 
    and "pixel depth":
        pxlDepth (1 bytes).
    Those are obviously a must for many things. One issue i had was other 
    formats storing the pixel depth in 'bits per pixel' i.e. for a 24bpp image 
    pxlDepth would be 24. In the GLi format we store the pixel depth in bytes. 
    So for a 24bpp image, pxlDepth will be 3 (8bits per pixel).

    5) I wanted a way to quickly get the size of the raw uncompressed image 
    data. width * height * pxlDepth make up this value. However instead of 
    having to compute that on load, we have a variable: 
        imgSize (4 bytes)
    which holds the size of our raw uncompressed pixel data in bytes.

    6) The next 2 fields weren't really problems in other formats, this is just 
    how i decided they would work best. It's easier IMHO to have 2 valid 
    C-strings of known size to give the author of the image their due credit, 
    as well as a time stamp for when the image was created. This gives us our 
    next two fields:  
        author (24 bytes)
    and: 
        stamp (28 bytes)
    Both can hold 1 less than the given value, that is to save 1 byte for the 
    terminating null character.

    7) We wanted to be able to compress the pixel data as much as possible. 
    However since the target was texture mapping and such in OpenGL the last 
    thing we want is lossy compression. Therefor we acknowledge the fact that 
    we won't get as good of compression ratios, but we can still greatly reduce 
    the size of our images raw pixel data, and keep the same excellent quality. 
    To do that we compress JUST the pixel data with the zlib lossless 
    compression library. (NOTE: Upon loading an image the pixel data is ALWAYS 
    uncompressed. When the function 'gli_load' returns a valid structure the 
    image data has already been uncompressed for you, assuming the image was 
    compressed.) To signal we have compressed data our header structures
    variable: 
        comp (comp[ressed]) (1 byte) 
    will be set to:
        GLI_IMAGE_COMPRESSED. 
    Otherwise it should hold the value: 
        GLI_IMAGE_NOTCOMPRESSED. 
    NOTE: Any other values (besides GLI_IMAGE_COMPRESSED) are assumed to mean 
    the image is NOT compressed. If we do have GLI_IMAGE_COMPRESSED then our 
    next headers variable:  
        compSize (comp[ressed]Size) (4 bytes)
    will hold the size of our compressed pixel data (in bytes). NOTE: We also 
    specify 2 #define's named: 
        GLI_IMAGE_COMPRESS
    and it's counterpart:
        GLI_IMAGE_DONTCOMPRESS
    They should be used when telling a function who's preparing an image 
    structure, to compress the data, or not to compress the data. That function 
    should then set the structures 'comp' variable to: GLI_IMAGE_COMPRESSED, 
    or GLI_IMAGE_NOTCOMPRESSED respectively. 

---------- Up until now this is what we consider our gli_image structures 
           "header" it is a total of 79 bytes. After this there's only 1 more 4 
           byte variable. When reading in the entire header do NOT include that 
           4 byte variable. To make that easier to remember ALWAYS use the 
           supplied #define:
                GLI_HEADER_SIZE .
           
    8) Lastly we wanted our pixel data to ALWAYS be in a known place. We 
    therefor didn't include any optional sections like some other formats do. 
    So the last thing our structure has is a variable named: 
        image (4 bytes). 
    Which is a pointer to some area of memory we'll allocate and store the 
    (raw) pixel data into.
    
    NOTE: This format was an experiment i happened to find useful. It was not
    meant to be a replace all, widely used image format. I could have used a
    stripped targa loader, or bitmap loader etc... but where's the fun in that?
    :-) If it does become popular great, i'd be glad to see other's benefit from
    my work in any way shape or form. However let it be known my intentions 
    were to create a very strait forward, simple, easy to use and understand,
    image file format for my texture mapping needs. I feel this has accomplished
    that goal quite well.
    
You may modify and or use this library in any way you see fit so long 
as the following is met:
        
[ USING AND MODIFYING THE LIBRARY ]
  The libraries presented here (libGLi and libTGA), and all other accompanying
  "tools" (in the tools_src/ directory) are subject to the LGPL license. A copy
  of that has been shipped w/ this library, and is in the file: LGPL.license
  Please refer to it for rules about modifying this code, and using any of these
  libraries in your free, or proprietary applications.
    
Constructive critisism is always welcome. Please send comments and or questions
to:  altf2o@gmail.com , please remember to use: GLi Comment\Question  in the
"Subject" line of the email. Makes my life easier. That aside, i hope someone
finds this useful, i currently use it for all my texture mapping needs.

FMorales...


[UPDATE 7/20/2010]
  I have updated my system to Ubuntu 10.04 x86_64. As of this writing this suite
  of utilities and libraries works flawlessly on both 32 and 64bit platforms.
  One thing i did have to do is add a couple packages so my system was ready
  to build:

    sudo apt-get install zlib1g-dev libglut3-dev

  Also once built, we need to add our ./lib/ path to our searchable library path
  so our applications can find it. For testing, during my current session, I
  simply use the following while using my Bash shell:

    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/full/path/to/libGLi/lib/

  If you had any issues running 'make' or running any applications, retry now, it
  should work just fine.

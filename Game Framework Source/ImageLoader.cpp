//
//  ImageLoader.cpp
//  Game Framework
//
//  Created by Ben Menke on 12/9/10
//  Copyright 2010 Menke. All rights reserved.
//

#include "ImageLoader.h"
#include "Image.h"
#include "FreeImage.h"

using namespace Framework;

bool Framework::loadImage(Image& image,const char* path)
{ 
    FREE_IMAGE_FORMAT format;
    
    format=FreeImage_GetFileType(path);
    if(format==FIF_UNKNOWN)
        return false;
    
    FIBITMAP* bitmap=FreeImage_Load(format, path);
    
    //bits per pixel
    int bpp=FreeImage_GetBPP(bitmap);
    int width=FreeImage_GetWidth(bitmap);
    int height=FreeImage_GetHeight(bitmap);
    BYTE* data=FreeImage_GetBits(bitmap);
    
    image.init(width, height, bpp, data);
    FreeImage_Unload(bitmap);
    return true;
}
#pragma once
//
//  Image.h
//  Game Framework
//
//  Created by Ben Menke on 12/12/10
//  Copyright 2010 Menke. All rights reserved.
//

#include "Resource.h"
#ifdef _WIN32
#include <d3d9.h>
#endif

namespace Framework 
{
    //opengl wrapper for 2d drawing
    
    class Image : public Resource
    {
        friend class Graphics;
    private:
#ifdef __APPLE__
        unsigned int texture;
#elif _WIN32
		IDirect3DTexture9* texture;
#endif
        int width;
        int height;
        
    public:
        void init(int width, int height, int colorDepth, void* data);
        
        int getWidth();
        int getHeight();
        
        bool load();
        bool release();
    };

}
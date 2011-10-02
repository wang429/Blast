//
//  Image.cpp
//  Game Framework
//
//  Created by Ben Menke on 12/12/10
//  Copyright 2010 Menke. All rights reserved.
//

#include "ImageLoader.h"
#include "Image.h"
#include "Engine.h"

using namespace Framework;

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>

void Image::init(int width, int height, int colorDepth, void* data)
{
	this->width=width;
	this->height=height;
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if(colorDepth>24)
		glTexImage2D(GL_TEXTURE_2D, 0, colorDepth/8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	else 
		glTexImage2D(GL_TEXTURE_2D, 0, colorDepth/8, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    loaded=true;
}

bool Image::release()
{
	glDeleteTextures(1,&texture);
    loaded=false;
    return true;
}

#elif _WIN32
#include <d3d9.h>
#include <d3dx9.h>
void Image::init(int width, int height, int colorDepth, void* data)
{
	this->width=width;
	this->height=height;
	
	IDirect3DDevice9* device=gEngine.getGraphicsDevice();
	if(colorDepth==32)
	{
		device->CreateTexture(width,height,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&texture,NULL);
	}
	else if(colorDepth==24)
	{
		device->CreateTexture(width,height,1,0,D3DFMT_R8G8B8,D3DPOOL_MANAGED,&texture,NULL);
	}
	
	D3DLOCKED_RECT rect;
	texture->LockRect(0,&rect,NULL,0);
	memcpy(rect.pBits,data,width*height*colorDepth/8);
	texture->UnlockRect(0);
}

bool Image::release()
{
	texture->Release();
    return true;
}
#endif

int Image::getWidth()
{
	return width;
}

int Image::getHeight()
{
	return height;
}

bool Image::load()
{
    return loadImage(*this, path.c_str());
}
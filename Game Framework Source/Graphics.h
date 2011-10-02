#pragma once
//
//  Graphics.h
//  Game Framework
//
//  Created by Ben Menke on 12/8/10
//  Copyright 2010 Menke. All rights reserved.
//

#include "Vector.h"
#include "Rect.h"

#ifdef __APPLE__

#include <OpenGL/OpenGL.h>

//blend mode defines
#define SRC_COLOR GL_SRC_COLOR
#define SRC_ALPHA GL_SRC_ALPHA
#define ONE GL_ONE
#define ZERO GL_ZERO
#define ONE_MINUS_SRC_ALPHA GL_ONE_MINUS_SRC_ALPHA
#define ONE_MINUS_SRC_COLOR	GL_ONE_MINUS_SRC_COLOR
#define DST_COLOR GL_DST_COLOR
#define DST_ALPHA GL_DST_ALPHA
#define ONE_MINUS_DST_ALPHA GL_ONE_MINUS_DST_ALPHA
#define ONE_MINUS_DST_COLOR GL_ONE_MINUS_DST_COLOR
#define SRC_ALPHA_SATURATE GL_SRC_ALPHA_SATURATE

//draw mode defines
#define POINTS GL_POINTS
#define TRIANGLES GL_TRIANGLES
#define LINES GL_LINES
#define TRIANGLE_STRIP GL_TRIANGLE_STRIP
#define LINE_STRIP GL_LINE_STRIP
#define TRIANGLE_FAN GL_TRIANGLE_FAN

#elif _WIN32

#include <d3d9.h>
#include <d3dx9.h>

#define SRC_ALPHA D3DBLEND_SRCALPHA
#define SRC_COLOR D3DBLEND_SRCCOLOR
#define ONE D3DBLEND_ONE
#define ZERO D3DBLEND_ZERO
#define ONE_MINUS_SRC_ALPHA D3DBLEND_INVSRCALPHA
#define ONE_MINUS_SRC_COLOR	D3DBLEND_INVSRCCOLOR
#define DST_COLOR D3DBLEND_DSTCOLOR
#define DST_ALPHA D3DBLEND_DSTALPHA
#define ONE_MINUS_DST_ALPHA D3DBLEND_INVDESTALPHA
#define ONE_MINUS_DST_COLOR D3DBLEND_INVDESTCOLOR
#define SRC_ALPHA_SATURATE D3DBLEND_SRCALPHASAT

//draw mode defines
#define POINTS D3DPT_POINTLIST
#define TRIANGLES D3DPT_TRIANGLELIST
#define LINES D3DPT_LINELIST
#define TRIANGLE_STRIP D3DPT_TRIANGLESTRIP
#define LINE_STRIP D3DPT_LINESTRIP
#define TRIANGLE_FAN D3DPT_TRIANGLEFAN
#endif

namespace Framework
{
    class Image;
    
#pragma pack(push,1)
    struct Color
    {
		//directx is currently using a bgr color structure
		//opengl is using a rgb
#ifdef __APPLE__
        unsigned char r,g,b,a;
#elif _WIN32
		unsigned char b,g,r,a;
#endif
    };
#pragma pack(pop)
    
    const Color colorWhite={255,255,255,255};
    const Color colorBlack={0,0,0,255};
    
#pragma pack(push, 1)
    struct Vertex
    {
        float x,y,z;
        Color c;
        float u,v;
    };
#pragma pack(pop)
    
    class Graphics
    {
    private:
#ifdef __APPLE__
        unsigned int texBuffer;
        unsigned int currentTex;
#elif _WIN32
		IDirect3DDevice9* device;
		IDirect3DVertexBuffer9* texBuffer;
		IDirect3DTexture9* currentTexture;
		ID3DXMatrixStack* worldStack;
		ID3DXMatrixStack* textureStack;
		D3DCOLOR clearColor;
#endif
        IRect resolution;
        IRect viewport;
        unsigned int drawMode;
    public:
        Graphics();
        
        void init();
        void deinit();

		void beginDraw();
		void endDraw();
        
        void setClearColor(float r, float g, float b);
        void setViewport(int x, int y, int width, int height);
        void setResolution(int width, int height);
        void setBlendMode(int func1, int func2);
        
        void setDrawMode(unsigned int drawMode); // sets the draw mode for drawing calls.
        void setPointSize(int size);
        void setImage(Image* image);
        
        void drawShape(Vertex* vertexes, int numVerts); //shape must be convex vertexes in CCW order
        void drawImage(Image* image);
        void drawImage(Image* image, IRect srcRect);
        
        //transform matrices
        void pushMatrix();
        void popMatrix();
        void pushTextureMatrix();
        void popTextureMatrix();
        
        void loadIdentity();
        void textureLoadIdentity();
        
        void setTextureTranslate(double x, double y);
        void setTextureRotation(double deg);
        void setTextureScale(double scaleX, double scaleY);
        
        void setTranslate(double x, double y);
        void setRotation(double deg);
        void setScale(double scaleX, double scaleY);
        
		//resets render settings
		void reset();
        
        void clear();
        void flush();
        
        IRect getResolution();
        
    };
    
    extern Graphics gGraphics;
}
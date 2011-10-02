//
//  Graphics.cpp
//  Game Framework
//
//  Created by Ben Menke on 12/8/10
//  Copyright 2010 Menke. All rights reserved.
//

#define NULL 0

#include "Graphics.h"
#include "Engine.h"
#include "Image.h"
#include "FreeImage.h"

using namespace Framework;
#ifdef __APPLE__
Graphics::Graphics()
{
	texBuffer=0;
	currentTex=0;
}

void Graphics::init()
{	
    FreeImage_Initialise();
    
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);	
	glEnable(GL_TEXTURE_2D);  
	glEnable(GL_POINT_SPRITE_ARB);
	glPointSize(64.0);
	glTexEnvf (GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glPointParameterf(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glPolygonMode(GL_BACK,GL_FILL);
	
	//create texture drawing buffer
	glGenBuffers(1, &texBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	float verts[16]={0,0,0,0,0,1.0,0,1.0,1.0,1.0,1.0,1.0,1.0,0,1.0,0};
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*16, verts, GL_STATIC_DRAW);
    
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glViewport(0, 0, 640, 480);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 0, 480, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	
	viewport.x=0;
	viewport.y=0;
	viewport.width=640;
	viewport.height=480;
	
	resolution.x=0;
	resolution.y=0;
	resolution.width=640;
	resolution.height=480;
	
	drawMode=TRIANGLE_FAN;
}

void Graphics::deinit()
{
    FreeImage_DeInitialise();
}

void Graphics::beginDraw()
{
}

void Graphics::endDraw()
{
}

void Graphics::setClearColor(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
}

void Graphics::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	
	viewport.x=x;
	viewport.y=y;
	viewport.width=width;
	viewport.height=height;
    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	
	resolution.width=width;
	resolution.height=height;
}

void Graphics::setResolution(int width, int height)
{	
    CGLContextObj ctx=CGLGetCurrentContext();
    GLint size[2] = {width, height};
    CGLSetParameter(ctx, kCGLCPSurfaceBackingSize, size);
    CGLEnable (ctx, kCGLCESurfaceBackingSize);
}

void Graphics::setBlendMode(int func1, int func2)
{
	glBlendFunc(func1, func2);
}

void Graphics::setPointSize(int size)
{
	glPointSize(size);
}

void Graphics::setDrawMode(unsigned int drawMode)
{
	this->drawMode=drawMode;
}

void Graphics::setImage(Image* image)
{
	if(image!=NULL)
		glBindTexture(GL_TEXTURE_2D, image->texture);
	
	else 
		glBindTexture(GL_TEXTURE_2D, 0);

}

void Graphics::drawShape(Vertex* vertexes, int numVerts)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), vertexes);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (char*)vertexes+sizeof(float)*3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (char*)vertexes+sizeof(char)*4+sizeof(float)*3);
	
	glDrawArrays(drawMode, 0, numVerts);
}

void Graphics::drawImage(Image* image)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(image->width, image->height, 1);
	
	glBindTexture(GL_TEXTURE_2D, image->texture);
	
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glVertexPointer(2, GL_FLOAT, 16, NULL);
	glTexCoordPointer(2, GL_FLOAT, 16, (void*)8);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glPopMatrix();
}

void Graphics::drawImage(Image* image, IRect srcRect)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glMatrixMode(GL_TEXTURE);
    glPushMatrix();
	glLoadIdentity();
    FRect texRect(srcRect.x/(float)image->width, srcRect.y/(float)image->height,srcRect.width/(float)image->width, srcRect.height/(float)image->height);
	glTranslatef(texRect.x, texRect.y, 0);
	glScalef(texRect.width, texRect.height, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScalef(srcRect.width, srcRect.height, 1);
	
	glBindTexture(GL_TEXTURE_2D, image->texture);
	
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glVertexPointer(2, GL_FLOAT, 16, NULL);
	glTexCoordPointer(2, GL_FLOAT, 16, (void*)8);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glPopMatrix();
    
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

void Graphics::pushMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void Graphics::popMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Graphics::pushTextureMatrix()
{
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
}

void Graphics::popTextureMatrix()
{
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
}

void Graphics::loadIdentity()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Graphics::textureLoadIdentity()
{
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
}

void Graphics::setTextureTranslate(double x, double y)
{
	glMatrixMode(GL_TEXTURE);
	glTranslatef(x, y, 0);
}

void Graphics::setTextureRotation(double deg)
{
	glMatrixMode(GL_TEXTURE);
	glRotated(deg, 0, 0, 1);
}

void Graphics::setTextureScale(double scaleX, double scaleY)
{
	glMatrixMode(GL_TEXTURE);
	glScaled(scaleX, scaleY, 1);
}

void Graphics::setTranslate(double x, double y)
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(x, y, 0);
}

void Graphics::setRotation(double deg)
{
	glMatrixMode(GL_MODELVIEW);
	glRotated(deg, 0, 0, 1);
}

void Graphics::setScale(double scaleX, double scaleY)
{
	glMatrixMode(GL_MODELVIEW);
	glScaled(scaleX, scaleY, 1);
}

void Graphics::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::flush()
{
	glFlush();
}

IRect Graphics::getResolution()
{
	return resolution;
}
#elif _WIN32
Graphics::Graphics()
{
	
}

#define fvfTexture (D3DFVF_XYZ|D3DFVF_TEX1)
#define fvfShape (D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE)

void Graphics::reset()
{
	device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_POINTSIZE,64.0);
	device->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
    device->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE );
    device->SetRenderState(D3DRS_LIGHTING,FALSE );
	device->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
    device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	device->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
}

void Graphics::init()
{	
    FreeImage_Initialise();
	device=gEngine.getGraphicsDevice();
	reset();

	clearColor=D3DCOLOR_RGBA(255,0,0,255);

	//create texture drawing buffer
	float verts[20]={0,0,0,0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,1,0};
	void* mem;
	device->CreateVertexBuffer(20*sizeof(float),D3DUSAGE_WRITEONLY,fvfTexture,D3DPOOL_MANAGED,&texBuffer,NULL);
	
	texBuffer->Lock(0,0,&mem,0);
	memcpy(mem,verts,sizeof(float)*20);
	texBuffer->Unlock();
	
	D3DVIEWPORT9 view;
	view.X=0;
	view.Y=0;
	view.Width=640;
	view.Height=480;
	view.MinZ=0.0f;
	view.MaxZ=1.0f;
	device->SetViewport(&view);
	
	D3DXMATRIX viewmatrix;
	D3DXMATRIX projection;
	D3DXVECTOR3 pos,target,up;
	pos=D3DXVECTOR3(0,0,1.0);
	target=D3DXVECTOR3(0,0,0.0);
	up=D3DXVECTOR3(0,1.0,0);
	D3DXMatrixLookAtRH(&viewmatrix,&pos,&target,&up);
	D3DXMatrixOrthoRH(&projection,640,480,0,1.0);
	//device->SetTransform(D3DTS_VIEW,&viewmatrix);
	device->SetTransform(D3DTS_PROJECTION,&projection);
	
	viewport.x=0;
	viewport.y=0;
	viewport.width=640;
	viewport.height=480;
	
	resolution.x=0;
	resolution.y=0;
	resolution.width=640;
	resolution.height=480;

	//initialize matrix stacks
	D3DXCreateMatrixStack(0,&worldStack);
	D3DXCreateMatrixStack(0,&textureStack);
}

void Graphics::deinit()
{
    FreeImage_DeInitialise();
	worldStack->Release();
	textureStack->Release();
	texBuffer->Release();
	device->Release();
}

void Graphics::beginDraw()
{
	device->BeginScene();
}

void Graphics::endDraw()
{
	device->EndScene();
}

void Graphics::setClearColor(float r, float g, float b)
{
	clearColor=D3DCOLOR_RGBA((int)(r*255),(int)(g*255),(int)(b*255),255);
}

void Graphics::setViewport(int x, int y, int width, int height)
{
	viewport.x=x;
	viewport.y=y;
	viewport.width=width;
	viewport.height=height;
	
	D3DVIEWPORT9 view;
	view.X=x;
	view.Y=y;
	view.Width=width;
	view.Height=height;
	view.MinZ=0.0f;
	view.MaxZ=1.0f;
	device->SetViewport(&view);
	D3DXMATRIX viewmatrix;
	D3DXMATRIX projection;
	D3DXVECTOR3 pos,target,up;
	pos=D3DXVECTOR3(0,0,1.0);
	target=D3DXVECTOR3(0,0,0.0);
	up=D3DXVECTOR3(0,1.0,0);
	D3DXMatrixLookAtRH(&viewmatrix,&pos,&target,&up);
	D3DXMatrixOrthoOffCenterRH(&projection,x,width,y,height,0,1.0);
	//device->SetTransform(D3DTS_VIEW,&viewmatrix);
	device->SetTransform(D3DTS_PROJECTION,&projection);
}

void Graphics::setResolution(int width, int height)
{	
	
}

void Graphics::setBlendMode(int func1, int func2)
{
	device->SetRenderState(D3DRS_SRCBLEND,func1);
	device->SetRenderState(D3DRS_DESTBLEND,func2);
}

void Graphics::setPointSize(int size)
{
	device->SetRenderState(D3DRS_POINTSIZE,size);
}

void Graphics::setDrawMode(unsigned int drawMode)
{
	this->drawMode=drawMode;
}

void Graphics::setImage(Image* image)
{
	if(image!=NULL)
		device->SetTexture(0,image->texture);
	else
		device->SetTexture(0,NULL);
}

void Graphics::drawShape(Vertex* vertexes, int numVerts)
{
	device->SetFVF(fvfShape);
	int primCount=0;
	switch(drawMode)
	{
	case TRIANGLE_FAN:
		primCount=numVerts-2;
		break;
	case TRIANGLES:
		primCount=numVerts/3;
		break;
	case TRIANGLE_STRIP:
		primCount=numVerts-2;
		break;
	case LINES:
		primCount=numVerts/2;
		break;
	case LINE_STRIP:
		primCount=numVerts-1;
		break;
	case POINTS:
		primCount=numVerts;
		break;
	default:
		return;
	}

	device->DrawPrimitiveUP((D3DPRIMITIVETYPE)drawMode,primCount,vertexes,24);
}

void Graphics::drawImage(Image* image)
{
	device->SetFVF(fvfTexture);
	
	pushTextureMatrix();
	textureLoadIdentity();
	
	pushMatrix();
	setScale(image->width, image->height);
	
	device->SetTexture(0,image->texture);
	
	device->SetStreamSource(0,texBuffer,0,20);
	
	device->DrawPrimitive(TRIANGLE_FAN,0,2);
	popTextureMatrix();
	popMatrix();
} 

void Graphics::drawImage(Image* image, IRect srcRect)
{
	device->SetFVF(fvfTexture);
    

    pushTextureMatrix();
    textureLoadIdentity();
	setTextureTranslate(srcRect.x/(double)image->width, srcRect.y/(double)image->height);
	setTextureScale(srcRect.width/(double)image->width, srcRect.height/(double)image->height);
	
    pushMatrix();
	D3DXMATRIX matrix;
	D3DXMATRIX outMatrix;
	D3DXMatrixScaling(&matrix,srcRect.width,srcRect.height,1);
	D3DXMatrixMultiply(&outMatrix,&matrix,worldStack->GetTop());
	device->SetTransform(D3DTS_WORLD,&outMatrix);
	
	device->SetTexture(0,image->texture);
	
	device->SetStreamSource(0,texBuffer,0,20);
	
	device->DrawPrimitive(TRIANGLE_FAN,0,2);
	popTextureMatrix();
	popMatrix();

}

void Graphics::pushMatrix()
{
	worldStack->Push();
}

void Graphics::popMatrix()
{
	worldStack->Pop();
	device->SetTransform(D3DTS_WORLD,worldStack->GetTop());
}

void Graphics::pushTextureMatrix()
{
	textureStack->Push();
}

void Graphics::popTextureMatrix()
{
	textureStack->Pop();
	device->SetTransform(D3DTS_TEXTURE0,textureStack->GetTop());
}

void Graphics::loadIdentity()
{
	worldStack->LoadIdentity();
	device->SetTransform(D3DTS_WORLD,worldStack->GetTop());
}

void Graphics::textureLoadIdentity()
{
	textureStack->LoadIdentity();
	device->SetTransform(D3DTS_TEXTURE0,textureStack->GetTop());
}

void Graphics::setTextureTranslate(double x, double y)
{
	D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix,x,y,0);
	D3DXMATRIX resultMatrix;
	D3DXMatrixMultiply(&resultMatrix,&matrix,textureStack->GetTop());
	textureStack->LoadMatrix(&resultMatrix);
	device->SetTransform(D3DTS_TEXTURE0,textureStack->GetTop());
}

void Graphics::setTextureRotation(double deg)
{
	D3DXMATRIX matrix;
	D3DXMatrixRotationZ(&matrix,deg*PI/180);
	D3DXMATRIX resultMatrix;
	D3DXMatrixMultiply(&resultMatrix,&matrix,textureStack->GetTop());
	textureStack->LoadMatrix(&resultMatrix);
	device->SetTransform(D3DTS_TEXTURE0,textureStack->GetTop());
}

void Graphics::setTextureScale(double scaleX, double scaleY)
{
	D3DXMATRIX matrix;
	D3DXMatrixScaling(&matrix,scaleX,scaleY,1);
	D3DXMATRIX resultMatrix;
	D3DXMatrixMultiply(&resultMatrix,&matrix,textureStack->GetTop());
	textureStack->LoadMatrix(&resultMatrix);
	device->SetTransform(D3DTS_TEXTURE0,textureStack->GetTop());
}

void Graphics::setTranslate(double x, double y)
{
	D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix,x,y,0);
	D3DXMATRIX resultMatrix;
	D3DXMatrixMultiply(&resultMatrix,&matrix,worldStack->GetTop());
	worldStack->LoadMatrix(&resultMatrix);
	device->SetTransform(D3DTS_WORLD,worldStack->GetTop());
}

void Graphics::setRotation(double deg)
{
	D3DXMATRIX matrix;
	D3DXMatrixRotationZ(&matrix,deg*PI/180);
	D3DXMATRIX resultMatrix;
	D3DXMatrixMultiply(&resultMatrix,&matrix,worldStack->GetTop());
	worldStack->LoadMatrix(&resultMatrix);
	device->SetTransform(D3DTS_WORLD,worldStack->GetTop());
}

void Graphics::setScale(double scaleX, double scaleY)
{
	D3DXMATRIX matrix;
	D3DXMatrixScaling(&matrix,scaleX,scaleY,1);
	D3DXMATRIX resultMatrix;
	D3DXMatrixMultiply(&resultMatrix,&matrix,worldStack->GetTop());
	worldStack->LoadMatrix(&resultMatrix);
	device->SetTransform(D3DTS_WORLD,worldStack->GetTop());
}

void Graphics::clear()
{
	device->Clear(0,0,D3DCLEAR_TARGET,clearColor,0,0);
}

void Graphics::flush()
{
	device->Present(NULL,NULL,NULL,NULL);
}

IRect Graphics::getResolution()
{
	return IRect();
}

#endif

Graphics Framework::gGraphics;

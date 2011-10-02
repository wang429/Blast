//
//  Engine.cpp
//  Game Framework
//
//  Created by Ben Menke on 8/14/11
//  Copyright 2011 Menke. All rights reserved.
//

#ifdef _WIN32

#include "Engine.h"
#include "Graphics.h"
#include "Game.h"
#include <d3d9.h>
using namespace Framework;

//window msg process
LRESULT CALLBACK windowProcess(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
   LRESULT result=false;			
	switch(msg)
	{
		case WM_KEYDOWN:
		{
			unsigned int code=(BYTE)wparam;

			if(gEngine.getKeyboard()->keyPressed((Key)code))
				gEngine.onKeyRepeat(0,code);
			else
				gEngine.onKeyPress(0,code);

			result=1;
			break;
		}
		case WM_KEYUP:
		{
			
			unsigned int code=(BYTE)wparam;
			gEngine.onKeyRelease(0,code);

			result=1;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			gEngine.onMousePress(MOUSE_LEFT);
			result=1;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			gEngine.onMousePress(MOUSE_RIGHT);
			result=1;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			gEngine.onMousePress(MOUSE_MIDDLE);
			result=1;
			break;
		}
		case WM_LBUTTONUP:
		{
			gEngine.onMouseRelease(MOUSE_LEFT);
			result=1;
			break;
		}
		case WM_RBUTTONUP:
		{
			gEngine.onMouseRelease(MOUSE_RIGHT);
			result=1;
			break;
		}
		case WM_MBUTTONUP:
		{
			gEngine.onMouseRelease(MOUSE_MIDDLE);
			result=1;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			result=1;
			break;
		}
		case WM_ACTIVATE:
		{
			bool active=wparam != WA_INACTIVE;
			if(active)
			{
				gEngine.onFocusGained();
			}
			else
			{
				gEngine.onFocusLost();
			}
			break;
		}
		case WM_CLOSE:
		{
			gEngine.end();
			break;
		}
		default: 
		{
			result=DefWindowProc (hwnd, msg, wparam, lparam);
			break;
		}
	}
	return result;
}

void Engine::init(Game* game, int screenWidth, int screenHeight, bool fullscreen, const char* title)
{
	currentTime=0;
	lastTime=0;
	//60fps
	interval=100.0/6.0;
	screenDimension=IRect(0,0,screenWidth,screenHeight);
	windowDimension=IRect(0,0,screenWidth,screenHeight);
	this->fullscreen=fullscreen;
	this->title=title;
	this->game=game;
	fps=0;
	running=false;
	quitting=false;
	background=false;


	HINSTANCE hinstance=GetModuleHandle(NULL);
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
    wc.style          = NULL;
    wc.lpfnWndProc    = (WNDPROC)windowProcess;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hinstance;
    wc.hIcon          = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground  = NULL;
    wc.lpszMenuName   = NULL;
    wc.lpszClassName  = title;
    wc.hIconSm        = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	RegisterClassEx(&wc);

	
	RECT windowRect;
	windowRect.bottom=windowDimension.height;
	windowRect.left=0;
	windowRect.top=0;
	windowRect.right=windowDimension.width;
	AdjustWindowRect(&windowRect,WS_OVERLAPPEDWINDOW,false);
	window=CreateWindowEx(0,title, title,WS_OVERLAPPEDWINDOW , CW_USEDEFAULT,CW_USEDEFAULT,windowRect.right-windowRect.left,windowRect.bottom-windowRect.top,NULL,NULL,hinstance,NULL);

	D3DPRESENT_PARAMETERS parameters;
	
	ZeroMemory( &parameters, sizeof(parameters) );
	parameters.BackBufferWidth=screenDimension.width;
	parameters.BackBufferHeight=screenDimension.height;
	parameters.BackBufferFormat=D3DFMT_A8R8G8B8;
	parameters.BackBufferCount=2;
	parameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
	parameters.hDeviceWindow=window;
	parameters.Windowed=!fullscreen;
	parameters.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;

	IDirect3D9* d3d=Direct3DCreate9(D3D_SDK_VERSION);
	HRESULT result=d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &parameters, &graphicsDevice); 

	gGraphics.init();
	gGraphics.setViewport(0,0,screenDimension.width,screenDimension.height);
	//gGraphics
}

void Engine::setRunsInBackground(bool yesNo)
{
	background=yesNo;
}
        
void Engine::setInterval(double milliesPerFrame)
{
	interval=milliesPerFrame;
}
        
void Engine::start()
{
	run();
}
void Engine::run()
{
	running=true;
	MSG msg;
	game->init();
	PeekMessage(&msg,NULL,0,0, PM_NOREMOVE);
	ShowWindow(window,SW_SHOW);
	while(!quitting)
	{
		currentTime=getHighResTime();
		sleep(interval-(currentTime-lastTime)/10000.0);
		while(currentTime-lastTime<interval)
		{
			currentTime=getHighResTime();
		}
		fps=1000.0/(currentTime-lastTime);

		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				quitting=true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		double deltaTime=currentTime-lastTime;
		if(deltaTime<0)
			deltaTime=interval;

		if(!paused)
			game->update(deltaTime);

		keyboard.updateKeys();
		mouse.updateButtons();
	}
	game->shutdown();
	running=false;

}
void Engine::end()
{ 
	quitting=true;
	PostQuitMessage(0);
}
        
int Engine::getFPS()
{
	return fps;
} 
double Engine::getInterval()
{
	return interval;
}
bool Engine::isQuitting()
{
	return quitting;
}
bool Engine::isRunning()
{
	return running;
}
        
bool Engine::isFullscreen()
{
	return fullscreen;
}
IRect Engine::getScreenDimensions()
{
	return screenDimension;
}
IRect Engine::getWindowDimensions()
{
	return windowDimension;
}
        
void Engine::setFullscreen(bool fullscreen)
{
	fullscreen=fullscreen;
	D3DPRESENT_PARAMETERS parameters;
	ZeroMemory( &parameters, sizeof(parameters) );
	parameters.BackBufferWidth=screenDimension.width;
	parameters.BackBufferHeight=screenDimension.height;
	parameters.BackBufferFormat=D3DFMT_A8R8G8B8;
	parameters.BackBufferCount=2;
	parameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
	parameters.hDeviceWindow=window;
	parameters.Windowed=!fullscreen;
	parameters.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;

	graphicsDevice->Reset(&parameters);
	gGraphics.reset();
}

void Engine::setScreenDimensions(int width, int height)
{
	screenDimension.width=width;
	screenDimension.height=height;
	D3DPRESENT_PARAMETERS parameters;
	ZeroMemory( &parameters, sizeof(parameters) );
	parameters.BackBufferWidth=screenDimension.width;
	parameters.BackBufferHeight=screenDimension.height;
	parameters.BackBufferFormat=D3DFMT_A8R8G8B8;
	parameters.BackBufferCount=2;
	parameters.SwapEffect=D3DSWAPEFFECT_DISCARD;
	parameters.hDeviceWindow=window;
	parameters.Windowed=!fullscreen;
	parameters.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;

	graphicsDevice->Reset(&parameters);
	gGraphics.reset();
	if(!fullscreen)
	{
		RECT windowRect;
		RECT adjustedRect;
		GetWindowRect(window,&windowRect);
		adjustedRect.left=0;
		adjustedRect.top=0;
		adjustedRect.bottom=screenDimension.height;
		adjustedRect.right=screenDimension.width;
		AdjustWindowRect(&windowRect,WS_OVERLAPPEDWINDOW,false);
		MoveWindow(window,windowRect.left,windowRect.top,adjustedRect.right,adjustedRect.bottom,true);
	}
}

int Engine::getTime()
{
	return timeGetTime();
}
double Engine::getHighResTime()
{
	LARGE_INTEGER time;
	LARGE_INTEGER frequency;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&frequency);
	double result=(double)time.QuadPart/(double)frequency.QuadPart;
	return result*1000;
}
void Engine::sleep(double millies)
{
	if(millies>0)
		MsgWaitForMultipleObjects(0,NULL,false,millies,0);				
}
        
Keyboard* Engine::getKeyboard()
{
	return &keyboard;
}
Mouse* Engine::getMouse()
{
	return &mouse;
}
        
void Engine::setGame(Game* game)
{
	this->game=game;
}

Game* Engine::getGame()
{
	return game;
}
        
void Engine::onKeyPress(char character, unsigned short key)
{
	keyboard.onKeyPress(character, key);
}
void Engine::onKeyRelease(char character, unsigned short key)		
{
	keyboard.onKeyRelease(character, key);
}
void Engine::onKeyRepeat(char character, unsigned short key)
{
	keyboard.onKeyRepeat(character, key);
}
        
void Engine::onMousePress(unsigned short button)
{
	mouse.onButtonPress(button);
}
void Engine::onMouseRelease(unsigned short button)
{
	mouse.onButtonRelease(button);
}
        
void Engine::onFocusLost()
{
	if(!background)
		paused=true;
}
void Engine::onFocusGained()
{
	paused=false;
}
        
bool Engine::openFileDialog(char* returnPath, int maxLength)
{
	return false;
}
bool Engine::saveFileDialog(char* returnPath, int maxLength)
{
	return false;
}

IDirect3DDevice9* Engine::getGraphicsDevice()
{
	return graphicsDevice;
}

HWND Engine::getHwnd()
{
	return window;
}

Engine Framework::gEngine;


#endif
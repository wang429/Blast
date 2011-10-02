//
//  Engine.mm
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2010 Menke. All rights reserved.
//

#include "App.h"
#include "Game.h"
#include "Engine.h"
#include "Keyboard.h"
#include "Graphics.h"
#include "Mouse.h"

using namespace Framework;

Engine Framework::gEngine;

void Engine::init(Game* game, int screenWidth, int screenHeight, bool fullscreen, const char* title)
{
	Engine::game=game;	
	Engine::fullscreen=fullscreen;
	Engine::screenDimension.width=screenWidth;
	Engine::screenDimension.height=screenHeight;
	Engine::windowDimension.width=screenWidth;
	Engine::windowDimension.height=screenHeight;
	Engine::title=title;
	Engine::interval=1000.0/60.0;
	Engine::fps=0;
	Engine::running=false;
	Engine::quitting=false;
	Engine::background=false;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	App* app=(App*)[App sharedApplication];
    
	NSRect frame=NSMakeRect(0, 0, screenWidth, screenHeight);
	Window* window=[[Window alloc] initWithContentRect:frame styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask backing:NSBackingStoreBuffered defer:YES];
	
	[window setTitle:[NSString stringWithCString:title encoding:NSASCIIStringEncoding]];
	
	[window setDelegate:window];
	
	NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFADoubleBuffer, 0
	};
	NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	
	NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:frame pixelFormat: pixelFormat];
	[window setContentView: view];
    int swap=0;
    [[view openGLContext] setValues:&swap forParameter:NSOpenGLCPSwapInterval];

    CGLContextObj ctx=CGLGetCurrentContext();
    GLint size[2] = {frame.size.width, frame.size.height};
    CGLSetParameter(ctx, kCGLCPSurfaceBackingSize, size);
    CGLEnable (ctx, kCGLCESurfaceBackingSize);
    
	app->window=window;
	
	NSString* path=[[NSBundle mainBundle] bundlePath];
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:path];
	
    
    gGraphics.init();
    
	[pool release];
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
	[NSApp run];
}

void Engine::run()
{
	//sleep for some fraction of the time remaining in the frame
    //allows the program to keep from wasting clock cycles
    currentTime=[NSDate timeIntervalSinceReferenceDate]*1000;
    [NSThread sleepForTimeInterval:(Engine::getInterval()-(currentTime-lastTime))/10000];
    while (currentTime-lastTime<interval)
    {
        currentTime=[NSDate timeIntervalSinceReferenceDate]*1000;
    }
	
	fps=1000/(currentTime-lastTime);
    
	//update current time and delta
	currentTime=[NSDate timeIntervalSinceReferenceDate]*1000;
    
	double deltaTime=currentTime-lastTime;
	
	//if time has wrapped(incredibly unlikely but possible) set deltaTime to the engine's interval
	if(currentTime<lastTime)
		deltaTime=interval;
	
	//update game
	if(!paused)
		game->update(deltaTime);
	
	keyboard.updateKeys();
	mouse.updateButtons();
	
	//update last time
	lastTime=currentTime;
	currentTime=[NSDate timeIntervalSinceReferenceDate]*1000;
	
}

void Engine::end()
{
	Engine::quitting=true;
	[NSApp terminate:NSApp];
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
	App* app=(App*)NSApp;
	Window* window=(Window*)app->window;
	
    // Get the screen information.
    NSScreen* mainScreen = [NSScreen mainScreen];
    NSDictionary* screenInfo = [mainScreen deviceDescription];
    NSNumber* screenID = [screenInfo objectForKey:@"NSScreenNumber"];
    
    // Capture the screen.
    CGDirectDisplayID displayID = (CGDirectDisplayID)[screenID longValue];
    CGDisplayCapture(displayID);
    
    int32_t shieldLevel = CGShieldingWindowLevel();
    
	[window setStyleMask:NSBorderlessWindowMask];
	[window setLevel:shieldLevel];
	
	Engine::fullscreen=fullscreen;
	NSRect rect=[mainScreen frame];
    
    setScreenDimensions(rect.size.width, rect.size.width);
}

void Engine::setScreenDimensions(int width, int height)
{	
	App* app=(App*)NSApp;
	Window* window=(Window*)app->window;
	
	[window setStyleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask];
	
	NSRect screen=NSMakeRect(0,0, width, height);
	[window setContentSize:screen.size];
	
	[window setLevel:NSNormalWindowLevel];
	
	screenDimension.width=width;
	screenDimension.height=height;
}

int Engine::getTime()
{
	return (int)[NSDate timeIntervalSinceReferenceDate]*1000;
}

double Engine::getHighResTime()
{
	return (double)[NSDate timeIntervalSinceReferenceDate]*1000;
}

void Engine::sleep(double millies)
{
	[NSThread sleepForTimeInterval:millies/1000.0];
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
	Engine::game=game;
}

Game* Engine::getGame()
{
	return game;
}	

///////ALERT//////////
//Implement Keyboard
//And Mouse Interface
//////////////////////

void Engine::onKeyPress(char character, unsigned short key)
{
	keyboard.onKeyPress(character,key);
}

void Engine::onKeyRelease(char character, unsigned short key)
{
	keyboard.onKeyRelease(character,key);
}

void Engine::onKeyRepeat(char character, unsigned short key)
{
	keyboard.onKeyRepeat(character,key);
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
    NSOpenPanel* panel=[NSOpenPanel openPanel];

	long result;
	[panel setAllowsMultipleSelection:NO];
	result=[panel runModal];
	if(result==NSOKButton)
	{
        NSString* string=[[panel filenames] objectAtIndex:0];
        
        [string getCString:returnPath maxLength:maxLength encoding:NSASCIIStringEncoding];
        
        return true;
	}
    
    return false;
}

bool Engine::saveFileDialog(char* returnPath, int maxLength)
{
    NSSavePanel* panel=[NSSavePanel savePanel];
    
	long result;
    
	result=[panel runModal];
	if(result==NSOKButton)
	{
        NSString* string=[panel filename];
        
        [string getCString:returnPath maxLength:maxLength encoding:NSASCIIStringEncoding];
        
        return true;
	}
    
    return false;
}





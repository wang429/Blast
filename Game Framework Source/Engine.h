#pragma once
 //
 //  Engine.h
 //  Game Framework
 //
 //  Created by Ben Menke on 11/24/10.
 //  Copyright 2010 Menke. All rights reserved.
 //

#include "Keyboard.h"
#include "Mouse.h"
#include "Rect.h"
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#include <d3d9.h>
#endif

namespace Framework 
{

    class Game;
    class Keyboard;
    class Mouse;
    
    class Engine
    {
    private:
#ifdef _WIN32
		HWND window;
		IDirect3DDevice9* graphicsDevice;
#endif
        double currentTime;
        double lastTime;
        double interval;
        Game* game;
        Keyboard keyboard;
        Mouse mouse;
        const char* title;
        IRect screenDimension;
        IRect windowDimension;
        int fps;
        bool paused;
        bool running;
        bool quitting;
        bool fullscreen;
        bool background;
        
    public:
        
        //initialization
        void init(Game* game, int screenWidth, int screenHeight, bool fullscreen, const char* title);
        
        //system config
        void setRunsInBackground(bool yesNo); //default is false(no)
        void setInterval(double milliesPerFrame); //sets the fps cap default is 60fps(16.67 milliesPerFrame)
        
        //system runtime
        void start();
        void run();
        void end();
        
        //get system state
        int getFPS(); 
        double getInterval();
        bool isQuitting();
        bool isRunning();
        
        //get window state
        bool isFullscreen();
        IRect getScreenDimensions();
        IRect getWindowDimensions();
        
        //set window state
        void setFullscreen(bool fullscreen);
        void setScreenDimensions(int width, int height);
        
        //timing functions /*both high res and normal time return millies
        int getTime();
        double getHighResTime();
        void sleep(double millies);
        
        //input functions 
        Keyboard* getKeyboard();
        Mouse* getMouse();
        
        //game functions
        void setGame(Game* game);
        Game* getGame();
        
        //app interface functions
        void onKeyPress(char character, unsigned short key);
        void onKeyRelease(char character, unsigned short key);
        void onKeyRepeat(char character, unsigned short key);
        
        void onMousePress(unsigned short button);
        void onMouseRelease(unsigned short button);
        
        void onFocusLost();
        void onFocusGained();
        
        bool openFileDialog(char* returnPath, int maxLength);
        bool saveFileDialog(char* returnPath, int maxLength);
        
#ifdef _WIN32
		IDirect3DDevice9* getGraphicsDevice();
		HWND getHwnd();
#endif
    };
    
    //global Singleton object
    extern Engine gEngine;

}


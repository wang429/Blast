#pragma once
//
//  Keyboard.h
//  Game Framework
//
//  Created by Ben Menke on 12/7/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include "KeyMapping.h"
#include "KeyboardListener.h"
#include "List.h"

//key states signify whether the button/key is pressed, released or down or up or repeating
#define KS_UP 0
#define KS_DOWN 1
#define KS_PRESSED 2
#define KS_RELEASED 3
#define KS_REPEAT 4

namespace Framework
{

    class Keyboard
    {
    private:
        List<KeyboardListener> listeners;
        char key[256];
        
    public:
        Keyboard();
        
        void onKeyPress(char charater, char inKey);
        void onKeyRepeat(char charater, char inKey);
        void onKeyRelease(char charater, char inKey);
        
        void updateKeys();
        
        bool keyPressed(Key inKey);
        bool keyDown(Key inKey);
        bool keyUp(Key inKey);
        bool keyReleased(Key inKey);
        bool keyRepeated(Key inKey);
        
        void addListener(KeyboardListener* listener);
        void removeListener(KeyboardListener *listener);
        
        char keyToChar(Key key);
    };
    
}



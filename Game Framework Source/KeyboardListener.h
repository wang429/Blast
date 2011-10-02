#pragma once
//
//  KeyboardListener.h
//  Game Framework
//
//  Created by Ben Menke on 4/3/11.
//  Copyright 2011 Menke. All rights reserved.
//


//  Keyboard listener for responding to keyboard events

#include "KeyMapping.h"

namespace Framework
{
    
    class KeyboardListener
    {
    public:
        virtual ~KeyboardListener(){};
        
        virtual void onKeyPress(char character, Key key)=0;
        virtual void onKeyRepeat(char character, Key key)=0;
        virtual void onKeyRelease(char character, Key key)=0;
    };

}
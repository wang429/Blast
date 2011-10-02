//
//  Keyboard.cpp
//  Game Framework
//
//  Created by Ben Menke on 12/7/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include "Keyboard.h"

using namespace Framework;

Keyboard::Keyboard()
{
	for(int i=0; i<256; i++)
	{
		key[i]=KS_UP;
	}
}

bool Keyboard::keyPressed(Key inKey)
{
	return (key[inKey]==KS_PRESSED);
}
bool Keyboard::keyDown(Key inKey)
{
	return (key[inKey]==KS_PRESSED||key[inKey]==KS_DOWN||key[inKey]==KS_REPEAT);
}
bool Keyboard::keyRepeated(Key inKey)
{
	return (key[inKey]==KS_PRESSED||key[inKey]==KS_REPEAT);
}
bool Keyboard::keyUp(Key inKey)
{
	return (key[inKey]==KS_UP||key[inKey]==KS_RELEASED);
}
bool Keyboard::keyReleased(Key inKey)
{
	return (key[inKey]==KS_RELEASED);
}


void Keyboard::onKeyPress(char character, char inKey)
{
	if(key[inKey]!=KS_REPEAT&&key[inKey]!=KS_DOWN)
		key[inKey]=KS_PRESSED;
    
    for(Link<KeyboardListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        iter->element->onKeyPress(character, (Key)inKey);
    }
	
}
void Keyboard::onKeyRelease(char charater, char inKey)
{
	key[inKey]=KS_RELEASED;
    
    for(Link<KeyboardListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        iter->element->onKeyRelease(charater, (Key)inKey);
    }
}

void Keyboard::onKeyRepeat(char charater, char inKey)
{
	key[inKey]=KS_REPEAT;
    
    for(Link<KeyboardListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        iter->element->onKeyRepeat(charater, (Key)inKey);
    }
}

void Keyboard::updateKeys()
{		
	//update key states for released keys and mouse buttons
	for(int i=0; i<256; i++)
	{
		if(key[i]==KS_RELEASED)
			key[i]=KS_UP;
		else if(key[i]==KS_PRESSED||key[i]==KS_REPEAT)
			key[i]=KS_DOWN;
	}
}



void Keyboard::addListener(KeyboardListener* listener)
{
    listeners.push(listener);
}

void Keyboard::removeListener(KeyboardListener *listener)
{
    for(Link<KeyboardListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        if(iter->element==listener)
        {
            listeners.remove(iter);
            return;
        }
    }
}


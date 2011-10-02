//
//  Mouse.cpp
//  Game Framework
//
//  Created by Ben Menke on 8/30/11.
//  Copyright 2011 Menke. All rights reserved.
//

#ifdef _WIN32

#include "Mouse.h"
#include "Engine.h"
#include <Windows.h>
using namespace Framework;

Mouse::Mouse()
{
	for(int i=0; i<4; i++)
	{
		button[i]=MS_UP;
	}
}

bool Mouse::buttonPressed(MouseButton inButton)
{
	return (button[inButton]==MS_PRESSED);
}
bool Mouse::buttonDown(MouseButton inButton)
{
	return (button[inButton]==MS_PRESSED||button[inButton]==MS_DOWN);
}
bool Mouse::buttonUp(MouseButton inButton)
{
	return (button[inButton]==MS_UP||button[inButton]==MS_RELEASED);
}
bool Mouse::buttonReleased(MouseButton inButton)
{
	return (button[inButton]==MS_RELEASED);
}


void Mouse::onButtonPress(char inButton)
{
	
	if(button[inButton]==MS_PRESSED||button[inButton]==MS_DOWN)
		button[inButton]=MS_DOWN;
	else
		button[inButton]=MS_PRESSED;
    
    for(Link<MouseListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        iter->element->onButtonPress((MouseButton)inButton);
    }
	
}
void Mouse::onButtonRelease(char inButton)
{
	button[inButton]=MS_RELEASED;
    
    for(Link<MouseListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        iter->element->onButtonRelease((MouseButton)inButton);
    }
}
void Mouse::updateButtons()
{		
	//update Button states for released Buttons and mouse buttons
	for(int i=0; i<3; i++)
	{
		if(button[i]==MS_RELEASED)
			button[i]=MS_UP;
		else if(button[i]==MS_PRESSED)
			button[i]=MS_DOWN;
	}
}


Vector Mouse::getPosition()
{
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(gEngine.getHwnd(),&pos);
	pos.y=gEngine.getScreenDimensions().height-pos.y;
	
	return Vector(pos.x,pos.y);
}
double Mouse::getScroll()
{
	return mouseScroll;
}

void Mouse::addListener(MouseListener* listener)
{
    listeners.push(listener);
}

void Mouse::removeListener(MouseListener *listener)
{
    for(Link<MouseListener>* iter=listeners.begin(); iter!=listeners.end(); iter=iter->next)
    {
        if(iter->element==listener)
        {
            listeners.remove(iter);
            return;
        }
    }
}

#endif


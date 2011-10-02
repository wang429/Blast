#pragma once
//
//  Mouse.h
//  Game Framework
//
//  Created by Ben Menke on 12/7/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include "MouseMapping.h"
#include "List.h"
#include "MouseListener.h"
#include "Vector.h"

//mouse states signify whether the button is pressed, released or down or up
#define MS_UP 0
#define MS_DOWN 1
#define MS_PRESSED 2
#define MS_RELEASED 3

namespace Framework 
{

    class Mouse
    {
    private:
        Vector mousePosition;
        double mouseScroll;
        List<MouseListener> listeners;
        // sized to 4 for alignment
        char button[4];
        
    public:
        Mouse();
        
        void onButtonPress(char inButton);
        void onButtonRelease(char inButton);
        
        
        void updateButtons();
        
        bool buttonPressed(MouseButton inButton);
        bool buttonDown(MouseButton inButton);
        bool buttonUp(MouseButton inButton);
        bool buttonReleased(MouseButton inButton);
        
        Vector getPosition();
        double getScroll();
        
        void addListener(MouseListener* listener);
        void removeListener(MouseListener* listener);
        
    };

}
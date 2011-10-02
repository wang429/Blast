#pragma once
//
//  MouseListener.h
//  Game Framework
//
//  Created by Ben Menke on 4/3/11.
//  Copyright 2011 Menke. All rights reserved.
//

#include "MouseMapping.h"

namespace Framework
{
    
    class MouseListener
    {
    public:
        virtual ~MouseListener(){};
        
        virtual void onButtonPress(MouseButton button)=0;
        virtual void onButtonRelease(MouseButton button)=0;
    };

}
#pragma once
//
//  Rect.h
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include "Vector.h"

namespace Framework 
{
    
    struct DRect;
    struct FRect;
    
    //standard rect
    struct IRect
    {
        int x;
        int y;
        int width;
        int height;
        
        IRect();
        IRect(int x, int y, int width, int height);
        IRect(DRect& rect);
        IRect(FRect& rect);
    };
    
    //double precision rect
    struct DRect
    {
        double x;
        double y;
        double width;
        double height;
        
        DRect();
        DRect(double x, double y, double width, double height);
        DRect(IRect& rect);
        DRect(FRect& rect);
    };
    
    //single precision rect
    struct FRect
    {
        float x;
        float y;
        float width;
        float height;
        
        FRect();
        FRect(float x, float y, float width, float height);
        FRect(DRect& rect);
        FRect(IRect& rect);
    };
    
    __inline bool intersectPoint(Vector point, DRect rect)
    {
        if(point.x<rect.x) 
            return false;
        if(point.y<rect.y) 
            return false;
        if(point.x>rect.x+rect.width) 
            return false;
        if(point.y>rect.y+rect.height) 
            return false;
        
        return true;
    }
}
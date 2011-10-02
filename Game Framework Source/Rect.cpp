//
//  Rect.cpp
//  Game Framework
//
//  Created by Ben Menke on 4/3/11.
//  Copyright 2011 Menke. All rights reserved.
//

#include "Rect.h"
using namespace Framework;

IRect::IRect() : x(0), y(0), width(0), height(0)
{}
IRect::IRect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
{}
IRect::IRect(DRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{}
IRect::IRect(FRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{}


DRect::DRect() : x(0), y(0), width(0), height(0)
{}
DRect::DRect(double x, double y, double width, double height) : x(x), y(y), width(width), height(height)
{}
DRect::DRect(IRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{}
DRect::DRect(FRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{}


FRect::FRect() : x(0), y(0), width(0), height(0)
{}
FRect::FRect(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
{}
FRect::FRect(DRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{}
FRect::FRect(IRect& rect) : x(rect.x), y(rect.y), width(rect.width), height(rect.height)
{}
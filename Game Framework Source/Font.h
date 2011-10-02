#pragma once
//
//  Font.h
//  Game Framework
//
//  Created by Ben Menke on 3/12/11.
//  Copyright 2011 Menke. All rights reserved.
//

#include "Rect.h"
#include <string>

namespace Framework
{

    /*
     Characters are the building blocks of fonts
     width is their width when being drawn
     bounds are their location in the texture.
     */
    
    class Image;
    class Graphics;
    
    struct Character
    {  
        int width;
        IRect bounds;
        char character;
    };
    
    /*
     Fonts store a character array 
     and control the drawing of strings
     */
    class Font
    {
    private:
        Image* image;
        Character characters[256];    
        int height;
        
    public:
        Font();
        
        void setImage(Image* image);
        Image* getImage();
        
        void setCharacters(Character* characters, int numCharacters);
        void getCharacters(Character* charArray, int size);
        
        int getHeight();
        
        int getStringWidth(const std::string& string);
        
        void drawString(Graphics* graphics, const std::string& string);
        void drawStringCentered(Graphics* graphics, const std::string& string);  
        
    };
}


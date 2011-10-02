//
//  Font.cpp
//  Game Framework
//
//  Created by Ben Menke on 3/12/11.
//  Copyright 2011 Menke. All rights reserved.
//

#include "Graphics.h"
#include "Image.h"
#include "Font.h"

using namespace Framework;

Font::Font()
{
    for(int i=0; i<256; i++)
    {
        characters[i].width=-1;
        characters[i].character=0;
    }
    height=0;
}

void Font::setImage(Image* image)
{
    this->image=image;
}

Image* Font::getImage()
{
    return image;
}

int Font::getHeight()
{
    return height;
}

void Font::setCharacters(Character* characters, int numCharacters)
{
    for(int i=0; i<numCharacters; i++)
    {
        char id=characters[i].character;
        this->characters[id]=characters[i];
        
        if(height<characters[i].bounds.height)
            height=characters[i].bounds.height;
    }
}

void Font::getCharacters(Character* charArray, int size)
{
    for(int i=0, j=0; i<size&&j<256; j++)
    {
        if(characters[j].character==0)
            continue;
            
        charArray[i]=characters[i];
        i++;
    }
}

int Font::getStringWidth(const std::string& string)
{
    if(string.length()==0)
        return 0;
    
    int width=0;
    
    for(int i=0; i<string.length(); i++)
    {
        char id=string[i];
        
        //skip if no character data for the char
        if(characters[id].character==0)
            continue;
        
        width+=characters[id].width;
    }
    
    
    return width;
}

void Font::drawString(Graphics* graphics, const std::string& string)
{
    //iterate over string
    int position=0;
    for(int i=0; i<string.length(); i++)
    {
        char id=string[i];
        
        //skip if no character data for the char
        if(characters[id].character==0)
            continue;
        
        //set position to needed position
        graphics->pushMatrix();
            graphics->setTranslate(position, 0);
        
            //draw the character
            graphics->drawImage(image,characters[id].bounds);
        graphics->popMatrix();
        
        position+=characters[id].width;
    }
}

void Font::drawStringCentered(Graphics* graphics, const std::string& string)
{
    //first get the width of the string
    int width=getStringWidth(string);
    
    //translate to center of width and height
    graphics->pushMatrix();
    graphics->setTranslate(-width/2.0, -height/2.0);

    //iterate over string
    int position=0;
    for(int i=0; i<string.length(); i++)
    {
        char id=string[i];
        
        //skip if no character data for the char
        if(characters[id].character==0)
            continue;
        
        //set position to needed position
        graphics->pushMatrix();
        graphics->setTranslate(position, 0);
        
        //draw the character
        graphics->drawImage(image,characters[id].bounds);
        graphics->popMatrix();
        
        position+=characters[id].width;
    }
    
    //pop the centering matrix
    graphics->popMatrix();
}
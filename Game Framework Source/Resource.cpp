//
//  Resource.cpp
//  Game Framework
//
//  Created by Ben Menke on 3/18/11.
//  Copyright 2011 Menke. All rights reserved.
//

#include <stddef.h>
#include "Resource.h"

using namespace Framework;

Resource::Resource()
{
    loaded=false;
}

void Resource::setName(const std::string& name)
{
    this->name=name;
}
void Resource::setPath(const std::string& path)
{
    this->path=path;
}

std::string& Resource::getName()
{
    return name;
}

std::string& Resource::getPath()
{
    return path;
}

bool Resource::isLoaded()
{
    return loaded;
}
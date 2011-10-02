#pragma once
//
//  Resource.h
//  Game Framework
//
//  Created by Ben Menke on 3/18/11.
//  Copyright 2011 Menke. All rights reserved.
//


//  Resource Base class
//  Resources are anything that can be loaded in from
//  a file or data.
//  They can be managed by the resource manager.
//  Each type of resource is responsible for knowing
//  how to load itself from a file and from raw data.

#include <string>

namespace Framework
{
    
    class Resource
    {
    
    protected:
        std::string name; //name of the resource
        std::string path; //path to the resource including the resource file
        
        bool loaded; //marked when the resource is loaded and ready for use.
        
    public:
        Resource();
        virtual ~Resource(){};
        
        virtual bool load()=0;
        virtual bool release()=0;
        
        void setName(const std::string& name);
        void setPath(const std::string& path);
        
        std::string& getName();
        std::string& getPath();
        
        bool isLoaded();
    };

}
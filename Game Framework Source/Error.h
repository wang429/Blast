#pragma once
//
//  Error.h
//  Game Framework
//
//  Created by Ben Menke on 11/24/10
//  Copyright 2010 Menke. All rights reserved.
//

namespace Framework
{

    /*
     Error class
     fires errors to debug output and logs if needed
     contains human readable name and message. The last
     error can be gotten by calling getLast
     errorType is the general type used by the different logs 
     whereas errorName is the specific error
     errorLevel is the priority of the level and should be
     set using one of the above defines
     */
    
    class Error
    {
    private:
        static Error lastError;
        unsigned int errorId; 
        const char* errorName;
        const char* errorMessage;
        const char*  errorType;
        int errorLine;
        const char* errorFile;
        unsigned char errorLevel;
        
    public:
        static void fireError(Error error);
        static Error getLast();
        
        Error();
        Error(const char* name, const char*  message, const char* file, int line, const char* type, char level);
        
        const char* getName();
        const char* getMessage();
        const char*  getType();
        char getLevel();
        
        bool operator==(Error &err);
    };
}


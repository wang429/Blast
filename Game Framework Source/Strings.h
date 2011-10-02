//
//  Strings.h
//  Game Framework
//
//  Created by Ben Menke on 12/7/10.
//  Copyright 2011 Menke. All rights reserved.
//

namespace Framework 
{
    /*
     Hashes a string down to an int type.
     */
    unsigned int hashString(const char* string);
    
    /*
     Concatenates two strings together. string1 first then string2
     */
    void concat(const char* string1, const char* string2, const char* concat, int maxChars);

}
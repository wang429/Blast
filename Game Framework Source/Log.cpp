//
//  Log.cpp
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2011 Menke. All rights reserved.
//

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "Log.h"

//quick fix for snprintf on windows
#ifdef _WIN32
#define snprintf _snprintf
#endif

using namespace Framework;
using namespace std;

const char* getLevelString(char level);

const char* getLevelString(char level)
{
	switch (level) {
		case 0:
			return ":MESSAGE:";
		case 1:
			return ":WARNING: ";
		case 2:
			return ":ERROR: ";
		case 3:
			return ":CRITICAL_ERROR: ";
		case 4:
			return ":FATAL_ERROR: ";
		default:
			return "";
	}
}

void Log::setImmediateFlush(bool immediate)
{
	Log::immediate=immediate;
}

char* Log::getLastMessage()
{
	return backlog[numMessages];
}

void Log::logMessage(char level, const char* logType, const char* file, int line, const char* message, ...)
{
	
	va_list argList;
	va_start(argList,message);
    
	int numChars=0;
	numChars+=snprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,logType,"");
	numChars+=snprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,getLevelString(level),"");
	numChars+=vsnprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,message,argList);
	numChars+=snprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,"  ");
	numChars+=snprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,file,"");
	numChars+=snprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,":");
	numChars+=snprintf(&backlog[numMessages][numChars],MAX_CHARS_PER_MESSAGE-numChars,"%i",line);
	
	printf(&backlog[numMessages][0],"");
	printf("\n");
	
	numMessages+=1;
	
	va_end(argList);
	
	
	if(immediate||numMessages>=MAX_MESSAGE_IN_QUEUE)
	{
        numMessages=0;
    }
}
void Log::flushLog()
{
	if(numMessages!=0)
	{
		ofstream file("Log.txt",ios_base::app);
		for(int i=0; i<numMessages; i++)
		{
			file.write(backlog[i],strlen(backlog[i]));
		}
		file.close();
		numMessages=0;
	}
}

bool Log::immediate(false);
int Log::numMessages;
char Log::backlog[MAX_MESSAGE_IN_QUEUE][MAX_CHARS_PER_MESSAGE];
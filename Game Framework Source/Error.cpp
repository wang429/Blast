//
//  Error.cpp
//  Game Framework
//
//  Created by Ben Menke on 8/20/11.
//  Copyright 2011 Menke. All rights reserved.
//

#ifdef _WIN32

#include <Windows.h>
#include <stdlib.h>
#include "Strings.h"
#include "Log.h"
#include "Engine.h"
#include "Error.h"

using namespace Framework;

Error::Error()
{
}

Error::Error(const char* name, const char*  message, const char* file, int line, const char* type, char level)
{
	errorName=name;
	errorMessage=message;
	errorType=type;
	errorLevel=level;
	errorLine=line;
	errorFile=file;
	errorId=hashString(errorName);
}

void Error::fireError(Error error)
{
	Error::lastError=error;
	
	Log::getLog(error.errorType)->logMessage(error.errorLevel,error.errorFile,error.errorLine,error.errorMessage, "");
	
	if(error.errorLevel==ERR_FATAL_ERROR)
	{
		MessageBox(gEngine.getHwnd(),"A fatal error has occured. The game will now close.","Critical Error",MB_OK|MB_ICONERROR);
		std::abort();
	}
}

const char* Error::getName()
{
	return errorName;
}

const char* Error::getMessage()
{
	return errorMessage;
}

char Error::getType()
{
	return errorType;
}

char Error::getLevel()
{
	return errorLevel;
}

bool Error::operator==(Error &err)
{
	return (err.errorId==errorId);
}


Error Error::lastError;

#endif

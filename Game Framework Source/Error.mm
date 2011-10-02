//
//  Error.mm
//  Game Framework
//
//  Created by Ben Menke on 11/24/10
//  Copyright 2010 Menke. All rights reserved.
//


#import "Cocoa/Cocoa.h"
#include <stdlib.h>
#include "Strings.h"
#include "Log.h"
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
	
    Log::logMessage(error.errorLevel,error.errorType,error.errorFile,error.errorLine,error.errorMessage, "");
	
	if(error.errorLevel==ERR_FATAL_ERROR)
	{
		NSRunCriticalAlertPanel([NSString stringWithCString:error.errorName encoding:NSASCIIStringEncoding], @"A fatal error has occured. The game will now close.", @"Close", @"", @"");
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

const char* Error::getType()
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
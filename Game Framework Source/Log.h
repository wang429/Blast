//
//  Log.h
//  Game Framework
//
//  Created by Ben Menke on 11/24/10.
//  Copyright 2011 Menke. All rights reserved.
//

/*
 Logs errors and other messages
 Currently supports up to 8 types of logs.
 */


#define ERR_FATAL_ERROR 4
#define ERR_CRITICAL_ERROR 3
#define ERR_ERROR 2
#define ERR_WARNING 1
#define ERR_NO_ERROR 0

#define MAX_LOGS 8
#define MAX_CHARS_PER_MESSAGE 1024
#define MAX_MESSAGE_IN_QUEUE 32

namespace Framework
{
    
    class Log
    {
    private:
        static bool immediate;
        static int numMessages;
        static char backlog[MAX_MESSAGE_IN_QUEUE][MAX_CHARS_PER_MESSAGE];
        
    public:
        static void setImmediateFlush(bool immediate);
        
        //gets messages in the log
        static char* getLastMessage();
        
        //logs a message. errors automatically log a message when fired, but this may be used for general messages as well
        //uses same set of levels as errors.
        static void logMessage(char level, const char* logType, const char* file, int line, const char* message,...);
        
        //write logs to file
        static void flushLog();
    };
#ifdef DEBUG
#define LOG(level, logType, message, ...) Log::logMessage(level, logType, __FILE__, __LINE__, message, __VA_ARGS__)
#else
#define LOG(level, logType, message, ...)
#endif
}
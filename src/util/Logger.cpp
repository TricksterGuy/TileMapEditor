#include "Logger.hpp"
#include <sys/time.h>

std::unique_ptr<AbstractLogger> logger(new Logger());

void SetLogger(AbstractLogger* logobj)
{
    logger.reset(logobj);
}

inline char GetLogAbbrev(LogLevel level)
{
    switch(level)
    {
    case LogLevel::FATAL:
        return 'F';
    case LogLevel::DEBUG:
        return 'D';
    case LogLevel::WARNING:
        return 'W';
    case LogLevel::INFO:
        return 'I';
    case LogLevel::VERBOSE:
        return 'V';
    default:
        return '?';
    }
}

void AbstractLogger::Log(LogLevel level, const std::string& format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    Log(level, format, argptr);
    va_end(argptr);
}

void AbstractLogger::Log(LogLevel level, const std::string& format, va_list ap)
{
    if (level > log_level)
        return;

    timeval curTime;
    gettimeofday(&curTime, NULL);
    char buffer[128];
    strftime(buffer, 128, "%H:%M:%S", localtime(&curTime.tv_sec));
    char currentTime[128] = "";
    snprintf(currentTime, 128, "%s:%ld", buffer, curTime.tv_usec);
    (*out) << GetLogAbbrev(level) << "[" << currentTime << "] ";

    DoLog(level, format, ap);
}

void Logger::DoLog(LogLevel level, const std::string& format, va_list ap)
{
    char buffer[1024];
    vsnprintf(buffer, 1024, format.c_str(), ap);
    (*out) << buffer << "\n";
}

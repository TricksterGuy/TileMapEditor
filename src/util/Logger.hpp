/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2017 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <cstdarg>
#include <memory>
#include <chrono>

enum class LogLevel
{
    FATAL_LEVEL = 0,   // printed and stops program
    DEBUG_LEVEL = 1,
    WARNING_LEVEL = 2,
    INFO_LEVEL = 3,
    VERBOSE_LEVEL = 4,
};

class AbstractLogger
{
public:
    AbstractLogger(std::ostream* target = &std::cout) : out(target), log_level(LogLevel::INFO_LEVEL), log_time(true) {}
    virtual ~AbstractLogger() {}
    virtual void Log(LogLevel level, const char* format, va_list ap);
    virtual void DoLog(LogLevel level, const char* format, va_list ap) {}
    void SetLogTarget(std::ostream* stream) { out = stream; }
    void SetLogLevel(LogLevel level) { log_level = level; }
    void SetLogTime(bool logging_time) { log_time = logging_time; }
protected:
    std::ostream* out;
    LogLevel log_level;
    bool log_time;
};

class Logger : public AbstractLogger
{
public:
    virtual void DoLog(LogLevel level, const char* format, va_list ap);
};

extern std::unique_ptr<AbstractLogger> logger;

void SetLogger(AbstractLogger* logobj);

static inline void Log(LogLevel level, const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    logger->Log(level, format, argptr);
    va_end(argptr);
}

static inline void Log(LogLevel level, const char* format, va_list arg)
{
    logger->Log(level, format, arg);
}

static inline void DebugFatalLog(const char* format, ...)
{
#ifdef DEBUG
    va_list argptr;
    va_start(argptr, format);
    Log(LogLevel::FATAL_LEVEL, format, argptr);
    va_end(argptr);
#endif
}

static inline void FatalLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    Log(LogLevel::FATAL_LEVEL, format, argptr);
    va_end(argptr);
}

static inline void DebugLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    Log(LogLevel::DEBUG_LEVEL, format, argptr);
    va_end(argptr);
}

static inline void WarnLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    Log(LogLevel::WARNING_LEVEL, format, argptr);
    va_end(argptr);
}

static inline void InfoLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    Log(LogLevel::INFO_LEVEL, format, argptr);
    va_end(argptr);
}

static inline void VerboseLog(const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    Log(LogLevel::VERBOSE_LEVEL, format, argptr);
    va_end(argptr);
}

/** Object that only exists to print out start and end of event call in a function */
class EventLog
{
public:
    EventLog(const char* function);
    ~EventLog();
private:
    const char* func;
    std::chrono::time_point<std::chrono::system_clock> startTime;
};

#endif

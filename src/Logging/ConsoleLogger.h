#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ILogger.h"

class ConsoleLogger : public virtual ILogger
{
    public:
        ConsoleLogger();
        ~ConsoleLogger();

        void LogMessage(LogEntry aLogEntry);
};

#endif
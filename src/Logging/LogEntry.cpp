#include <iomanip>
#include <sstream>

#include "LogEntry.h"

std::wstring LogEntry::ToString()
{
    const wchar_t* level;

    switch (LogLevel)
    {
        case ELogLevel::CRITICAL:    level = L" [CRITICAL] ";     break;
        case ELogLevel::WARNING:     level = L" [WARNING] ";      break;
        case ELogLevel::INFO:        level = L" [INFO] ";         break;
        case ELogLevel::DEBUG:       level = L" [DEBUG] ";        break;

        default:                    level = L" [TRACE] ";        break;
    }

    struct tm timeinfo;
    localtime_s(&timeinfo, (time_t*)&Timestamp);

    std::wstringstream oss;
    oss << std::put_time(&timeinfo, L"%Y-%m-%d %H:%M:%S");
    oss << std::setw(12) << level;
    std::wstring str = oss.str() + std::wstring{Message} + L"\n";

    return str;
}
///----------------------------------------------------------------------------------------------------
/// Copyright (c) Raidcore.GG - All rights reserved.
///
/// Name         :  ILogger.h
/// Description  :  Interface for custom loggers.
/// Authors      :  K. Bieniek
///----------------------------------------------------------------------------------------------------

#ifndef ILOGGER_H
#define ILOGGER_H

#include "ELogLevel.h"
#include "LogEntry.h"

#include <mutex>

///----------------------------------------------------------------------------------------------------
/// ILogger Interface
///----------------------------------------------------------------------------------------------------
class ILogger
{
	public:
		///----------------------------------------------------------------------------------------------------
		/// ctor
		///----------------------------------------------------------------------------------------------------
		ILogger()			= default;

		///----------------------------------------------------------------------------------------------------
		/// dtor
		///----------------------------------------------------------------------------------------------------
		virtual ~ILogger()	= default;

		///----------------------------------------------------------------------------------------------------
		/// GetLogLevel:
		/// 	Gets the log level.
		///----------------------------------------------------------------------------------------------------
		ELogLevel GetLogLevel();

		///----------------------------------------------------------------------------------------------------
		/// SetLogLevel:
		/// 	Sets the log level.
		///----------------------------------------------------------------------------------------------------
		void SetLogLevel(ELogLevel aLogLevel);

		///----------------------------------------------------------------------------------------------------
		/// LogMessage:
		/// 	Virtual function to implement custom logic for logging.
		///----------------------------------------------------------------------------------------------------
		virtual void LogMessage(LogEntry aLogEntry) = 0;

	protected:
		ELogLevel	LogLevel;
		std::mutex	MessageMutex;
};

#endif

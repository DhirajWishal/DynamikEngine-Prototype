#pragma once
#ifndef _DYNAMIK_SYSTEM_ERROR_FLAG_H
#define _DYNAMIK_SYSTEM_ERROR_FLAG_H

#include "DataTypesLib/Public/FastString.h"
#include "Console.h"

namespace Dynamik {
	enum class ErrorSeverity {
		NONE,
		CORE_INFO,
		CORE_WARN,
		CORE_ERROR,
		CORE_FETAL,
		APPLICATION_INFO,
		APPLICATION_WARN,
		APPLICATION_ERROR,
		APPLICATION_FETAL
	};

	struct FlagInfo {
		ErrorSeverity mySeverity = ErrorSeverity::NONE;
		FSTR myMessage = "";

		FSTR myFilePath = "";
		UI32 myLine = 0;
		FSTR myTimeStamp = "";
	};

	class ErrorFlagSystem {
	public:
		ErrorFlagSystem() {}
		~ErrorFlagSystem() {}

		static void Flag(FlagInfo& flg)
		{
			UI32 _severityIndex = 0;
			switch (flg.mySeverity)
			{
			case ErrorSeverity::CORE_INFO:
				_severityIndex = 1;
				break;
			case ErrorSeverity::CORE_WARN:
				_severityIndex = 2;
				break;
			case ErrorSeverity::CORE_ERROR:
				_severityIndex = 3;
				break;
			case ErrorSeverity::CORE_FETAL:
				_severityIndex = 4;
				break;
			case ErrorSeverity::APPLICATION_INFO:
				_severityIndex = 5;
				break;
			case ErrorSeverity::APPLICATION_WARN:
				_severityIndex = 6;
				break;
			case ErrorSeverity::APPLICATION_ERROR:
				_severityIndex = 7;
				break;
			case ErrorSeverity::APPLICATION_FETAL:
				_severityIndex = 8;
				break;
			default:
				break;
			}

			ErrorFlagSystem myEFS;
			myEFS.LOG(_severityIndex, flg.myMessage, flg.myFilePath, flg.myLine);
		}

		void InfoFlag(FlagInfo& flg)
		{
			UI32 _severityIndex = 0;
		}

	private:
		void LOG(int si, FSTR msg, FSTR file, UI32 line);
	};
}

#endif // !_DYNAMIK_SYSTEM_ERROR_FLAG_H

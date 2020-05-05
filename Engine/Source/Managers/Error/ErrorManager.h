#pragma once
#ifndef _DYNAMIK_ERROR_MANAGER_H
#define _DYNAMIK_ERROR_MANAGER_H

#include <string>
#include "Public/datatypes.h"

namespace Dynamik {
	enum class ErrorPriority {
		ERROR_PRIORITY_ERROR,
		ERROR_PRIORITY_INFO,
		ERROR_PRIORITY_WARN,
		ERROR_PRIORITY_QUESTION
	};

	class ErrorManager {
		ErrorManager() {}
		~ErrorManager() {}

	public:
		static UI32 flagMessage(std::string message, std::string title, ErrorPriority priority);
	};
}

#ifdef DMK_DEBUG
#define DMK_FLAG(message, title, priority) ErrorManager::flagMessage(message, title, priority)
#define DMK_FLAG_INFO(message)		DMK_FLAG(message, "Message!", ErrorPriority::ERROR_PRIORITY_INFO)
#define DMK_FLAG_QUESTION(message)	DMK_FLAG(message, "Message!", ErrorPriority::ERROR_PRIORITY_QUESTION)
#define DMK_FLAG_WARN(message)		DMK_FLAG(message, "Message!", ErrorPriority::ERROR_PRIORITY_WARN)
#define DMK_FLAG_ERROR(message)		DMK_FLAG(message, "Message!", ErrorPriority::ERROR_PRIORITY_ERROR) 

#else
#define DMK_FLAG(message, title, priority) 
#define DMK_FLAG_INFO(message)		
#define DMK_FLAG_QUESTION(message)	
#define DMK_FLAG_WARN(message)		
#define DMK_FLAG_ERROR(message)		

#endif // DMK_DEBUG


#endif // !_DYNAMIK_ERROR_MANAGER_H

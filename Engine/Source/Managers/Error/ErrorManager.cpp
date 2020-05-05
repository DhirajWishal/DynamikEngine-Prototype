#include "dmkafx.h"
#include "ErrorManager.h"

namespace Dynamik {
	UI32 ErrorManager::flagMessage(std::string message, std::string title, ErrorPriority priority)
	{
		switch (priority)
		{
		case Dynamik::ErrorPriority::ERROR_PRIORITY_ERROR:
			return MessageBox(NULL, message.c_str(), title.c_str(), MB_ICONERROR);
			break;
		case Dynamik::ErrorPriority::ERROR_PRIORITY_INFO:
			return MessageBox(NULL, message.c_str(), title.c_str(), MB_ICONINFORMATION);
			break;
		case Dynamik::ErrorPriority::ERROR_PRIORITY_WARN:
			return MessageBox(NULL, message.c_str(), title.c_str(), MB_ICONWARNING);
			break;
		case Dynamik::ErrorPriority::ERROR_PRIORITY_QUESTION:
			return MessageBox(NULL, message.c_str(), title.c_str(), MB_ICONQUESTION);
			break;
		}

		return 0;
	}
}
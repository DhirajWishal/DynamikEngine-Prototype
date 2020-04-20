#include "dmkafx.h"
#include "InternalThread.h"
#include "core/log.h"

namespace Dynamik {
	void InternalThreadHandler::init()
	{
		DMK_CORE_INFO("Internal Thread Handler Initiated!");
		DMK_CORE_INFO("Internal Thread Handler Main Loop!");
	}

	void InternalThreadHandler::loop()
	{
	}

	B1 InternalThreadHandler::loopEndCommand()
	{
		return false;
	}

	DMKThreadTreminateReason InternalThreadHandler::shutdown()
	{
		DMK_CORE_INFO("Internal Thread Handler Shutdowns!");
		return DMKThreadTreminateReason::DMK_THREAD_TERMINATION_REASON_NORMAL_TERMINATION;
	}
}
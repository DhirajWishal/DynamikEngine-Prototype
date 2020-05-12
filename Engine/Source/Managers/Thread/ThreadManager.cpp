#include "dmkafx.h"
#include "ThreadManager.h"

namespace Dynamik {
	static B1 isEventThreadRunning = false;

	static B1 closeEventThread = false;

	void ThreadManager::initiateThreads(std::vector<POINTER<InternalFormat>> formats, std::vector<POINTER<DMKWindow>> handles)
	{
		this->inFlightObjects = formats;
		this->windowHandles = handles;

		_eventThreadHandle->swap(std::thread(_eventThread, &this->inFlightObjects, &this->windowHandles));
	}

	void ThreadManager::updateInFlightObjects(std::vector<POINTER<InternalFormat>> formats)
	{
		this->inFlightObjects = formats;
	}

	void ThreadManager::updateWindowHandles(std::vector<POINTER<DMKWindow>> handles)
	{
		this->windowHandles = handles;
	}
	
	void ThreadManager::_eventThread(POINTER<std::vector<POINTER<InternalFormat>>> formats, POINTER<std::vector<POINTER<DMKWindow>>> windowHandles)
	{
		while (!closeEventThread)
		{
			for (auto _handle : windowHandles.dereference())
			{
				_handle->pollEvents();
			}
		}
	}
}
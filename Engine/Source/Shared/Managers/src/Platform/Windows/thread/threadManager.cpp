#include "mngafx.h"
#include "threadManager.h"
#include "ExecutionHandle.h"

/*
 Each thread consists of,
 * Initialization
 * Main loop
 * Shutdown
*/

namespace Dynamik {
	ThreadManager::ThreadManager()
	{
	}

	void ThreadManager::add(POINTER<Thread> myThread)
	{
		myThread->setIndex(myThreadCount++);
		myThreadContainer.push_back(myThread);
	}

	void ThreadManager::run(UI32 index)
	{
		if (index >= myThreadCount)
			return;

		ExecutionHandle _thread(runThread, myThreadContainer[index]);
	}

	void ThreadManager::runAll()
	{
		ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> _handles(myThreadContainer.size() + 1);
		_handles.push_back(std::async(std::launch::async, internalThread, &myInternalThreadHandler));
		for (POINTER<Thread> thread : myThreadContainer)
			_handles.push_back(std::async(std::launch::async, internalThread, thread.get()));
	}

	void ThreadManager::update(UI32 index, ThreadDataContainer container)
	{
		if (index >= myThreadCount)
			return;
	}

	void ThreadManager::runThread(POINTER<Thread> thread)
	{
		thread->init();
		while (!thread->loopEndCommand())thread->loop();
		thread->shutdown();
	}

	void ThreadManager::internalThread(POINTER<InternalThreadHandler> handler)
	{
		handler->init();
		while (!handler->loopEndCommand()) handler->loop();
		handler->shutdown();
	}

	bool ThreadManager::evaluateShutdown(DMKThreadTreminateReason reason)
	{
		return false;
	}
}
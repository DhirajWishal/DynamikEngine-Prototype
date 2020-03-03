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
	ThreadManager::ThreadManager() {
		myInternalThreadHandler.setIndex(myThreadCount++);
		ExecutionHandle _mainInternalThreadHandle(internalThread, &myInternalThreadHandler);
	}

	void ThreadManager::add(Thread* myThread)
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
		std::vector<ExecutionHandle*> _handles;
		for (Thread* thread : myThreadContainer)
			_handles.push_back(&ExecutionHandle(runThread, thread));
	}

	void ThreadManager::update(UI32 index, ThreadDataContainer container) {
		if (index >= myThreadCount)
			return;
	}

	void ThreadManager::runThread(Thread* thread)
	{
		thread->init();
		while (!thread->loopEndCommand())thread->loop();
		thread->shutdown();
	}

	void ThreadManager::internalThread(InternalThreadHandler* handler)
	{
		handler->init();
		while (!handler->loopEndCommand()) handler->loop();
		handler->shutdown();
	}
}
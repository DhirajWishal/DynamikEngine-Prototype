#include "mngafx.h"
#include "threadManager.h"

/*
 Each thread consists of,
 * Initialization
 * Main loop
 * Shutdown
*/

namespace Dynamik {
	ThreadManager::ThreadManager() {
		myThreadContainer.push_back(&internalThreadHandle);
	}

	void ThreadManager::add(Thread* myThread)
	{
		myThreadContainer.push_back(myThread);
		myThreadCount++;
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
		while (!thread->loopEndCommand())thread->mainLoop();
		thread->shutdown();
	}

	/* EXECUTION HANDLE CLASS */
	ExecutionHandle::ExecutionHandle(Thread* thread)
	{
		myThread.swap(std::thread([](Thread* _thr) {
			_thr->init();
			while (!_thr->loopEndCommand())_thr->mainLoop();
			_thr->shutdown(); }, thread));
	}

	ExecutionHandle::ExecutionHandle(std::thread thread)
	{
		myThread.swap(thread);
	}

	ExecutionHandle::~ExecutionHandle()
	{
		myThread.join();
	}
}
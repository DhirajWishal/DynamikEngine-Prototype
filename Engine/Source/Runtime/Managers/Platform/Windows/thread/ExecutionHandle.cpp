#include "dmkafx.h"
#include "ExecutionHandle.h"

namespace Dynamik {
	ExecutionHandle::ExecutionHandle(POINTER<Thread> thread)
	{
		myThread.swap(std::thread([](POINTER<Thread> _thr) {
			_thr->init();
			while (!_thr->loopEndCommand())_thr->loop();
			_thr->shutdown(); }, thread));
	}

	ExecutionHandle::ExecutionHandle(std::thread thread)
	{
		myThread.swap(thread);
	}

	ExecutionHandle::ExecutionHandle(ExecutionHandle& myHandle)
	{
		myThread.swap(myHandle.myThread);
	}

	ExecutionHandle::~ExecutionHandle()
	{
		myThread.join();
	}
}
#include "mngafx.h"
#include "ExecutionHandle.h"

namespace Dynamik {
	ExecutionHandle::ExecutionHandle(Thread* thread)
	{
		myThread.swap(std::thread([](Thread* _thr) {
			_thr->init();
			while (!_thr->loopEndCommand())_thr->loop();
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
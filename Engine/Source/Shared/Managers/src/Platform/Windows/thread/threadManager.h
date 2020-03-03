#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_MANAGER_H
#define _DYNAMIK_MANAGERS_THREAD_MANAGER_H

#include <thread>

#include "Thread.h"
#include "InternalThread.h"

namespace Dynamik {
	/*
	 THREAD MANAGER
	*/
	class DMK_API ThreadManager {
	public:
		ThreadManager();	// default constructor
		~ThreadManager() {}	// default destructor

		void add(Thread* myThread);	// add a thread
		void run(UI32 index = 0);	// run a thread with index
		void runAll();	// run all threads

		void update(UI32 index, ThreadDataContainer container);	// update a thread's data container

	private:
		static void runThread(Thread* thread);	// internal run thread function
		static void internalThread(InternalThreadHandler* handler);	// internal thread function

		std::vector<Thread*> myThreadContainer;	// thread container
		UI32 myThreadCount = 0;	// thread count
		InternalThreadHandler myInternalThreadHandler;	// internal thread handle
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_MANAGER_H

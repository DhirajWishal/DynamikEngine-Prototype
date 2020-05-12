#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_MANAGER_H
#define _DYNAMIK_MANAGERS_THREAD_MANAGER_H

#include <thread>

#include "Public/Thread.h"
#include "InternalThread.h"
#include "Public/Pointer.h"

namespace Dynamik {
	/*
	 THREAD MANAGER
	*/
	class ThreadManager {
	public:
		ThreadManager();	// default constructor
		~ThreadManager() {}	// default destructor

		void add(POINTER<Thread> myThread);	// add a thread
		void run(UI32 index = 0);	// run a thread with index
		void runAll();	// run all threads

		void update(UI32 index, ThreadDataContainer container);	// update a thread's data container

	private:
		static void runThread(POINTER<Thread> thread);	// internal run thread function
		static void internalThread(POINTER<InternalThreadHandler> handler);	// internal thread function
		static bool evaluateShutdown(DMKThreadTreminateReason reason);	// evaluate why the process terminated.

		std::vector<POINTER<Thread>> myThreadContainer;	// thread container
		UI32 myThreadCount = 0;	// thread count
		InternalThreadHandler myInternalThreadHandler;	// internal thread handle
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_MANAGER_H

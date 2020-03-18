#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_EXECUTION_HANDLE_H
#define _DYNAMIK_MANAGERS_THREAD_EXECUTION_HANDLE_H

#include "CentralDataHub.h"
#include "Platform.h"
#include "Thread.h"

namespace Dynamik {
	/*
	 EXECUTION HANDLE
	 Execution Handle for the Thread Manager
	*/
	class ExecutionHandle {
	public:
		template<class FUNC>
		ExecutionHandle(FUNC function)	// default constructor
		{
			myThread.swap(std::thread(function));
		}
		template<class FUNC, class ARGS>
		ExecutionHandle(FUNC function, ARGS arguments) // default constructor
		{
			myThread.swap(std::thread(function, arguments));
		}
		ExecutionHandle(POINTER<Thread> thread);	// default constructor
		ExecutionHandle(std::thread thread);	// default constructor
		ExecutionHandle(ExecutionHandle& myHandle);
		~ExecutionHandle();	// default destructor

		POINTER<std::thread> getThreadAddr() { return &myThread; }	// return the address of the thread
		B1 isJoinable() { return myThread.joinable(); }	// check if the thread is joinable
		std::thread::id getThreadID() { return myThread.get_id(); }	// get the thread ID

	private:
		std::thread myThread;	// thread
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_EXECUTION_HANDLE_H

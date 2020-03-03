#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_EXECUTION_HANDLE_H
#define _DYNAMIK_MANAGERS_THREAD_EXECUTION_HANDLE_H

#include <thread>

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
		ExecutionHandle(FUNC function) : myThread(std::thread(function)) {}	// default constructor
		template<class FUNC, class ARGS>
		ExecutionHandle(FUNC function, ARGS arguments) : myThread(std::thread(function, arguments)) {}	// default constructor
		ExecutionHandle(Thread* thread);	// default constructor
		ExecutionHandle(std::thread thread);	// default constructor
		~ExecutionHandle();	// default destructor

		std::thread* getThreadAddr() { return &myThread; }	// return the address of the thread
		B1 isJoinable() { return myThread.joinable(); }	// check if the thread is joinable
		std::thread::id getThreadID() { return myThread.get_id(); }	// get the thread ID

	private:
		std::thread myThread;	// thread
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_EXECUTION_HANDLE_H

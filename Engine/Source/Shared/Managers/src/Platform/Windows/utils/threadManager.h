#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_MANAGER_H
#define _DYNAMIK_MANAGERS_THREAD_MANAGER_H

#include <thread>

namespace Dynamik {
	/*
	 Data Container for Threads
	*/
	struct DMK_API ThreadDataContainer {
		VPTR myRawData = nullptr;
	};

	/*
	 Thread object
	*/
	class DMK_API Thread {
	public:
		Thread() {}
		virtual ~Thread() {}

		virtual void init() {}
		virtual void mainLoop() {}
		virtual B1 loopEndCommand() { return terminate; }
		virtual void shutdown() {}

		void setIndex(UI32 index) { myThreadIndex = index; }
		UI32 getIndex() { return myThreadIndex; }
		UI32 getID() { return myThreadID; }
		void setDataContainer(ThreadDataContainer* container) { myDataContainer = container; }

	protected:
		UI32 myThreadID = 0;
		UI32 myThreadIndex = 0;
		B1 terminate = false;

		ThreadDataContainer* myDataContainer = nullptr;
	};

	/*
	 Execution Handle for the Thread Manager
	*/
	class DMK_API ExecutionHandle {
	public:
		// default constructor
		template<class FUNC, class ARGS>
		ExecutionHandle(FUNC function, ARGS arguments) : myThread(std::thread(function, arguments)) {}
		// default constructor
		ExecutionHandle(Thread* thread);
		// default constructor
		ExecutionHandle(std::thread thread);
		// default destructor
		~ExecutionHandle();

		// return the address of the thread
		std::thread* getThreadAddr() { return &myThread; }
		// check if the thread is joinable
		B1 isJoinable() { return myThread.joinable(); }
		// get the thread ID
		std::thread::id getThreadID() { return myThread.get_id(); }

	private:
		std::thread myThread;	// thread
	};

	/*
	 Thread Manager
	*/
	class DMK_API ThreadManager {
	public:
		ThreadManager() {}	// default constructor
		~ThreadManager() {}	// default destructor

		void add(Thread* myThread);	// add a thread
		void run(UI32 index = 0);	// run a thread with index
		void runAll();	// run all threads

	private:
		static void runThread(Thread* thread);

		std::vector<Thread*> myThreadContainer;
		std::vector<ExecutionHandle> myThreadHandles;
		UI32 myThreadCount = 0;
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_MANAGER_H

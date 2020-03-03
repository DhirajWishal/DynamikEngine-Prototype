#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_INTERNAL_H
#define _DYNAMIK_MANAGERS_THREAD_INTERNAL_H

#include "Thread.h"

namespace Dynamik {
	/*
	 INTER-THREAD DATA CONTAINER
	 This operates as a data structure which sends data in between threads
	*/
	class InternalThreadDataContainer : public ThreadDataContainer {
	public:
		InternalThreadDataContainer() {}
		~InternalThreadDataContainer() {}
	};

	/*
	 INTERNAL THREAD HANDLER CLASS
	 This operates on another thread and manages all the other threads.
	*/
	class InternalThreadHandler : public Thread {
	public:
		InternalThreadHandler() {}
		~InternalThreadHandler() {}

		void init() override;
		void loop() override;
		B1 loopEndCommand() override;
		void shutdown() override;

	private:
		InternalThreadDataContainer myDataContainer;
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_INTERNAL_H

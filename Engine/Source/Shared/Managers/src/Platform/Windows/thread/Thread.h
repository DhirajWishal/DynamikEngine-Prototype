#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_H
#define _DYNAMIK_MANAGERS_THREAD_H

#include "CentralDataHub.h"
#include "Platform.h"

namespace Dynamik {
	/*
	 THREAD DATA CONTAINER (SUPER/ BASE/ PARENT CLASS)
	*/
	class DMK_API ThreadDataContainer {
		VPTR myRawData = nullptr;	// raw data pointer

		UI32 myThreadIndex = 0;	// destination thread index
	};

	/*
	 MAIN THREAD OBJECT (SUPER/ BASE/ PARENT CLASS)
	*/
	class DMK_API Thread {
	public:
		Thread() {}	// default constructor
		virtual ~Thread() {}	// default virtual destructor

		virtual void init() {}	// virtual init function
		virtual void loop() {}	// virtual main loop function
		virtual B1 loopEndCommand() { return terminate; }	// virtual end loop command
		virtual void shutdown() {}	// virtual shutdown function

		void setIndex(UI32 index) { myThreadIndex = index; }	// set index
		UI32 getIndex() { return myThreadIndex; }	// get current index
		UI32 getID() { return myThreadID; }	// get the thread ID
		void setDataContainer(ThreadDataContainer* container) { myDataContainer = container; } // set data container

	protected:
		virtual void update(ThreadDataContainer* container) {}	// update data container

		UI32 myThreadID = 0;	// thread ID
		UI32 myThreadIndex = 0;	// thread index
		B1 terminate = false;	// terminate command

		ThreadDataContainer* myDataContainer = nullptr;	// data container
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_H

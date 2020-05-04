#pragma once
#ifndef _DYNAMIK_DATA_TYPES_THREAD_BASE_H
#define _DYNAMIK_DATA_TYPES_THREAD_BASE_H

#include "Pointer.h"

namespace Dynamik {
	enum class DMKThreadTreminateReason {
		DMK_THREAD_TERMINATION_REASON_NORMAL_TERMINATION,
		DMK_THREAD_TERMINATION_REASON_TIMEOUT,
		DMK_THREAD_TERMINATION_REASON_INETERUPT,
		DMK_THREAD_TERMINATION_REASON_IO_REQUEST,
		DMK_THREAD_TERMINATION_REASON_CLOSE_EVENT
	};

	/*
	 THREAD DATA CONTAINER (SUPER/ BASE/ PARENT CLASS)
	*/
	class ThreadDataContainer {
		VPTR myRawData = nullptr;	// raw data pointer

		UI32 myThreadIndex = 0;	// destination thread index
	};

	/*
	 MAIN THREAD OBJECT (SUPER/ BASE/ PARENT CLASS)
	*/
	class Thread {
	public:
		Thread() {}	// default constructor
		virtual ~Thread() {}	// default virtual destructor

		virtual void init() {}	// virtual init function
		virtual void loop() {}	// virtual main loop function
		virtual B1 loopEndCommand() { return terminate; }	// virtual end loop command
		virtual DMKThreadTreminateReason shutdown() { return DMKThreadTreminateReason::DMK_THREAD_TERMINATION_REASON_NORMAL_TERMINATION; }	// virtual shutdown function

		void setIndex(UI32 index) { myThreadIndex = index; }	// set index
		UI32 getIndex() { return myThreadIndex; }	// get current index
		UI32 getID() { return myThreadID; }	// get the thread ID
		void setDataContainer(POINTER<ThreadDataContainer> container) { myDataContainer = container; } // set data container

	protected:
		virtual void update(POINTER<ThreadDataContainer> container) {}	// update data container

		UI32 myThreadID = 0;	// thread ID
		UI32 myThreadIndex = 0;	// thread index
		B1 terminate = false;	// terminate command

		POINTER<ThreadDataContainer> myDataContainer;	// data container
	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_H

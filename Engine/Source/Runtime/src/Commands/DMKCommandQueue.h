#pragma once
#ifndef _DYNAMIK_COMMAND_QUEUE_H
#define _DYNAMIK_COMMAND_QUEUE_H

/* HOW DYNAMIK COMMAND QUEUE WORKS
 In one specific location, a pointer to command queues are placed called as Command Pointer.
 This location will not be deallocated 
 until the Engine instance is destroyed. When a command is made by the client application, the command will be 
 inserted to a queue and stored in a specific location. And that address will be sent to the 
*/

#include "core/data store/DMKObject.h"
#include "DMKCommand.h"
#include "DataTypesLib/Public/Pointer.h"

namespace Dynamik {
	class DMKCommandQueue;

	struct DMKCommandQueuePointer {
		POINTER<DMKCommandQueue> pointer;
	};

	class DMKCommandQueue : public DMKObject {
	public:
		DMKCommandQueue() {}
		~DMKCommandQueue() {}

		void add(DMKCommand* command);

	private:
		ARRAY<DMKCommand*> myCommands;
	};

	typedef POINTER<DMKCommandQueuePointer> COMPTR;	// Pointer to the Command Queue Pointer
}

#endif // !_DYNAMIK_COMMAND_QUEUE_H

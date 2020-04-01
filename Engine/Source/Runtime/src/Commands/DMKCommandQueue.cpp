#include "dmkafx.h"
#include "DMKCommandQueue.h"

namespace Dynamik {
	void DMKCommandQueue::add(DMKCommand* command)
	{
		myCommands.pushBack(command);
	}
}
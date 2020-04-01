#include "dmkafx.h"
#include "DMKCommands.h"

namespace Dynamik {
	DMKCommands& DMKCommands::getInstance()
	{
		static DMKCommands myInstance;
		return myInstance;
	}
	
	/* PUBLIC STATIC FUNCTIONS AND DATA */
	COMPTR DMKCommands::getCommandQueuePointer()
	{
		return getInstance()._getCommandQueuePointer();
	}

	void DMKCommands::submitObject(COMPTR pointer, DMKSubmitObjectCommand descriptor)
	{
		pointer->pointer->add(&descriptor);
	}
	
	/* PRIVATE FUNCTIONS AND DATA */
	COMPTR DMKCommands::_getCommandQueuePointer()
	{
		return COMPTR();
	}
}
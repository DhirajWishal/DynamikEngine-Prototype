#pragma once
#ifndef _DYNAMIK_COMMAND_H
#define _DYNAMIK_COMMAND_H

#include "core/data store/DMKObject.h"

namespace Dynamik {
	enum class DMKCommandType {
		/* REQUEST COMMANDS */
		DMK_COMMAND_TYPE_REQUEST_EVENTS,
		DMK_COMMAND_TYPE_REQUEST_OBJECT,

		/* SUBMIT COMMANDS */
		DMK_COMMAND_TYPE_SUBMIT_OBJECT,

		/* UPDATE COMMANDS */
		DMK_COMMAND_TYPE_UPDATE_OBJECT,
	};

	class DMKCommand : public DMKObject {
	public:
		DMKCommand(DMKCommandType type) : myType(type) {}
		~DMKCommand() {}

		DMKCommandType myType;
	};
}

#endif // !_DYNAMIK_COMMAND_H

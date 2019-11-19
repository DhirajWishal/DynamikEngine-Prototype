#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_NETWORK_MESSAGE_TYPE_H
#define _DYNAMIK_CENTRAL_DATA_HUB_NETWORK_MESSAGE_TYPE_H

namespace Dynamik {
	enum class MessageType {
		RAW = 0,
		INIT = 1,
		COMMAND = 2,
		LOCATION = 3,
		STATUS = 4,
		STATUS_INFO = 5,
		AUDIO = 6
	};

	enum class MessageDataTypeNames {
		INT = 0,
		FLOAT = 1,
		DOUBLE = 2,
		CHAR = 3,
		STRING = 4,
		VOID_POINTER = 5
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_NETWORK_MESSAGE_TYPE_H

#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_NETWORK_MESSAGE_DATA_H
#define _DYNAMIK_CENTRAL_DATA_HUB_NETWORK_MESSAGE_DATA_H

#include "messageType.h"

namespace Dynamik {
	class MessageData {
	public:
		MessageData(void* data);
		template<typename Alloc>
		MessageData(Alloc data);
		~MessageData();

		template<typename Alloc>
		Alloc getData();

		template<typename T>
		MessageData& operator=(const T& value);
		MessageData& operator=(MessageData& value);

	private:
		void _set(const void* data);

		MessageType m_type = MessageType::RAW;
		void* m_data = nullptr;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_NETWORK_MESSAGE_DATA_H

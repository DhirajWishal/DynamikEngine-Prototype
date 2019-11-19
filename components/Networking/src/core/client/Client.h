#pragma once

#ifndef _DYNAMIK_NETWORKING_CLIENT_BASE_H
#define _DYNAMIK_NETWORKING_CLIENT_BASE_H

#include <string>
#include <queue>

#include "CentralDataHub.h"

namespace Dynamik {
	namespace Networking {
		class Client {
		public:
			Client() {}
			virtual ~Client() {}

			virtual int connect(std::string& ipAddress, int portNumber) {}
			virtual void send(MessageType type, std::string& message) {}
			virtual void receive() {}
			virtual MessageData getReceivedData() {}

			virtual void close() {}

		protected:
			std::string serverAddress = "";
			int port = 0000;

			std::queue<MessageData> sendDataQueue;
			std::queue<MessageData> receivedDataQueue;
		};
	}
}

#endif

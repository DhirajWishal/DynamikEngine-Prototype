#pragma once

#ifndef _DYNAMIK_NETWORKING_CLIENT_BASE_H
#define _DYNAMIK_NETWORKING_CLIENT_BASE_H

#include <string>
#include <queue>


namespace Dynamik {
	namespace Networking {
		class DMK_API Client {
		public:
			Client() {}
			virtual ~Client() {}

			virtual int connect(std::string& ipAddress, int portNumber) { return 0; }
			//virtual void send(MessageType type, std::string& message) {}
			//virtual void receive() {}
			//virtual MessageData getReceivedData() { return nullptr; }

			virtual void close() {}

		protected:
			std::string serverAddress = DMK_TEXT("");
			int port = 0000;

			//std::queue<MessageData> sendDataQueue;
			//std::queue<MessageData> receivedDataQueue;
		};
	}
}

#endif

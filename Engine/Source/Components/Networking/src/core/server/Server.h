#pragma once

#ifndef _DYNAMIK_NETWORKING_SERVER_BASE_H
#define _DYNAMIK_NETWORKING_SERVER_BASE_H

#include <string>
#include "core/interface.h"

constexpr auto DEFAULT_BUFFER_LENGTH = 512;

namespace Dynamik {
	namespace Networking {
		class Server {
		public:
			Server() {}
			virtual ~Server() {}

			virtual void init(std::string& ipAddress, int portNumber) {}
			virtual int setSocket() { return 0; }
			virtual void handleRequests() {}
			virtual void startListning() {}

			virtual void shutDown() {}

		protected:
			virtual void closeSocket(SOCKET& socket) {}
			virtual void cleanUp() {}

			std::string myIPAddress = "";
			int myPort = 0000;

			int iResult = 0;

			WSADATA wsaData = {};
			SOCKET listenSocket = INVALID_SOCKET;
			SOCKET clientSocket = INVALID_SOCKET;
			struct addrinfo* result = NULL;
			struct addrinfo* hints = NULL;

			int iSendResult = 0;
			char recvbuf[DEFAULT_BUFFER_LENGTH] = {};
			int recvbuflen = DEFAULT_BUFFER_LENGTH;
		};
	}
}

#endif

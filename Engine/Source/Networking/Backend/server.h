#pragma once

/*
 Dynamik Engine Networking Library

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		15/11/2019
 IDE:		MS Visual Studio Community 2019

 server.h
*/

#ifndef _DYNAMIK_NETWORKING_SERVER_H
#define _DYNAMIK_NETWORKING_SERVER_H

#include "server/Server.h"

namespace Dynamik {
	namespace Networking {
		class server : public Server {
		public:
			server() {}
			~server() {}

			void init(std::string& ipAddress, int portNumber) override;
			int setSocket() override;
			void startListning() override;

			void shutDown() override;

		private:
			void closeSocket(SOCKET& socket) override;
			void cleanUp() override;
		};
	}
}

#endif

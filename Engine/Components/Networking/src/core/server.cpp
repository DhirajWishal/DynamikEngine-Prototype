#include "netafx.h"
#include "server.h"

#include "CentralDataHub.h"
#include "core/log.h"

namespace Dynamik {
	namespace Networking {
		void server::init(std::string& ipAddress, int portNumber) {
			myIPAddress = ipAddress;
			myPort = portNumber;
		}

		int server::setSocket() {
			iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				DMK_CORE_FATAL("WSAStartup failed with error: " + std::to_string(iResult));
				return DMK_RESULT_UNSUCCESSFUL;
			}

			ZeroMemory(hints, sizeof(*hints));
			hints->ai_family = AF_INET;
			hints->ai_socktype = SOCK_STREAM;
			hints->ai_protocol = IPPROTO_TCP;
			hints->ai_flags = AI_PASSIVE;

			PCSTR port = std::to_string(myPort).c_str();

			iResult = getaddrinfo(NULL, port, hints, &result);
			if (iResult != 0) {
				DMK_CORE_FATAL("getaddrinfo failed with error: " + std::to_string(iResult));
				cleanUp();
				return DMK_RESULT_UNSUCCESSFUL;
			}

			listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (listenSocket == INVALID_SOCKET) {
				DMK_CORE_FATAL("socket failed with error: " + std::to_string(WSAGetLastError()));
				freeaddrinfo(result);
				cleanUp();
				return DMK_RESULT_UNSUCCESSFUL;
			}

			iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				DMK_CORE_FATAL("bind failed with error: " + std::to_string(WSAGetLastError()));
				freeaddrinfo(result);
				closeSocket(listenSocket);
				cleanUp();
				return DMK_RESULT_UNSUCCESSFUL;
			}

			freeaddrinfo(result);

			iResult = listen(listenSocket, SOMAXCONN);
			if (iResult == SOCKET_ERROR) {
				DMK_CORE_FATAL("listen failed with error: " + std::to_string(WSAGetLastError()));
				closeSocket(listenSocket);
				cleanUp();
				return 1;
			}

			sockaddr* address = NULL;

			// Accept a client socket
			clientSocket = accept(listenSocket, (sockaddr*)NULL, (int)NULL);
			if (clientSocket == INVALID_SOCKET) {
				DMK_CORE_FATAL("accept failed with error: " + std::to_string(WSAGetLastError()));
				closeSocket(listenSocket);
				cleanUp();
				return 1;
			}

			// No longer need server socket
			closeSocket(listenSocket);

			return DMK_RESULT_SUCCESSFUL;
		}

		void server::startListning() {
			// Receive until the peer shuts down the connection
			do {
				iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
				if (iResult > 0) {
					DMK_CORE_INFO("Bytes received: " + std::to_string(iResult));

					// Echo the buffer back to the sender
					iSendResult = send(clientSocket, recvbuf, iResult, 0);
					if (iSendResult == SOCKET_ERROR) {
						DMK_CORE_FATAL("send failed with error: " + std::to_string(WSAGetLastError()));
						closeSocket(clientSocket);
						cleanUp();
					}
					DMK_CORE_INFO("Bytes sent: " + std::to_string(iSendResult));
				}
				else if (iResult == 0)
					DMK_CORE_INFO("Connection closing...");
				else {
					DMK_CORE_FATAL("recv failed with error: " + std::to_string(WSAGetLastError()));
					closeSocket(clientSocket);
					cleanUp();
				}
			} while (iResult > 0);
		}

		void server::shutDown() {
			iResult = shutdown(clientSocket, SD_SEND);
			if (iResult == SOCKET_ERROR) {
				DMK_CORE_FATAL("shutdown failed with error: " + std::to_string(WSAGetLastError()));
				closeSocket(clientSocket);
				cleanUp();
			}

			closeSocket(clientSocket);
			cleanUp();
		}

		void server::closeSocket(SOCKET& socket) {
			closesocket(socket);
		}

		void server::cleanUp() {
			WSACleanup();
		}
	}
}
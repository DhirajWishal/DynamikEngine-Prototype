#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_FILE_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_FILE_MANAGER_H

#include "CentralDataHub.h"

namespace Dynamik {
	namespace manager {
		class file {
		public:
			enum mode {
				READ,
				WRITE,
				RW
			};

			explicit file(std::string path, mode myMode);
			~file();

			void open();
			std::fstream* getFile() { return myFile; }
			bool isOpen() { return myFile->is_open(); }
			void clearFile();
			void clear();
			void restore();
			void close();

		private:
			std::fstream* myFile = nullptr;
			std::fstream* backup = nullptr;

			std::string myPath = "";
			uint8 myMode = 0;
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_WINDOWS_FILE_MANAGER_H

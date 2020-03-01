#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_FILE_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_FILE_MANAGER_H

#include "CentralDataHub.h"
#include <fstream>
#include <string>

namespace Dynamik {
	namespace manager {
		class file {
		public:
			enum mode {
				READ,
				WRITE,
				RW
			};

			explicit file(std::wstring path, mode myMode);
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

			std::wstring myPath = L"";
			UI8 myMode = 0;
		};
	}
}

#endif // !_DYNAMIK_MANAGERS_WINDOWS_FILE_MANAGER_H

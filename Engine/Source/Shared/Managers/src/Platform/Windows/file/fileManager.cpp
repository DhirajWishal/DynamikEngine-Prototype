#include "mngafx.h"
#include "fileManager.h"

namespace Dynamik {
	namespace manager {
		file::file(std::string path, mode myMode) :
			myPath(path), myMode(myMode) {
		}

		file::~file() {
		}

		void file::open() {
			if (myMode == READ)
				myFile->open(myPath, std::ios::in);
			else if (myMode == WRITE)
				myFile->open(myPath, std::ios::out);
			else if (myMode == RW)
				myFile->open(myPath);
			else
				std::runtime_error("Invalid file mode setting!");
		}

		void file::close() {
			myFile->close();
		}

		void file::clear() {
			myFile->clear();
		}

		void file::clearFile() {
			backup = myFile;
			myFile->clear();
		}
	}
}
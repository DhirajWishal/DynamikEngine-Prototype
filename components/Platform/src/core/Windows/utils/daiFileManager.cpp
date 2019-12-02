#include <fstream>
#include <string>
#include <vector>
#include "daiFileManager.h"

namespace Dynamik {
	namespace utils {
		daiManager::daiManager() {
		}

		daiManager::daiManager(std::string& path) : myPath(path) {
			file.open(path);
		}

		daiManager::~daiManager() {
			file.close();
		}

		void daiManager::open(std::string path) {
			file.open(path);
		}

		bool daiManager::isOpen() {
			return file.is_open();
		}

		std::string daiManager::getLine() {
			std::string line = "";
			std::getline(file, line, '\n');

			return line;
		}

		std::istream& daiManager::getLine(std::string* line) {
			return std::getline(file, *line, '\n');
		}

		std::fstream& daiManager::getFile() {
			return file;
		}

		void daiManager::close() {
			file.close();
		}

		std::vector<std::string> daiManager::getData(DMKDaiFileDataType type) {
			if (!isDataStoreInit)
				initDataStore();

			return dataStore[type];
		}

		std::vector<std::vector<std::string>> daiManager::getAllData() {
			if (!isDataStoreInit)
				initDataStore();

			return dataStore;
		}

		void daiManager::initDataStore() {
			dataStore.resize(7);

			std::string _line = "";
			while (getLine(&_line)) {
				if (_line[0] == 'M' || _line[0] == 'm')
					dataStore[DMK_DAI_FILE_DATA_TYPE_MODEL].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'I' || _line[0] == 'i')
					dataStore[DMK_DAI_FILE_DATA_TYPE_TEXTURE].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'V' || _line[0] == 'v')
					dataStore[DMK_DAI_FILE_DATA_TYPE_VERTEX].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'T' || _line[0] == 't')
					dataStore[DMK_DAI_FILE_DATA_TYPE_TESSELLATION].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'G' || _line[0] == 'g')
					dataStore[DMK_DAI_FILE_DATA_TYPE_GEOMETRY].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'F' || _line[0] == 'f')
					dataStore[DMK_DAI_FILE_DATA_TYPE_FRAGMENT].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'S' || _line[0] == 's')
					dataStore[DMK_DAI_FILE_DATA_TYPE_SCRIPT].push_back((_line.substr(2, _line.size() - 2)));
			}

			file.clear();
			file.seekg(0, std::ios::beg);

			isDataStoreInit = true;
		}
	}
}
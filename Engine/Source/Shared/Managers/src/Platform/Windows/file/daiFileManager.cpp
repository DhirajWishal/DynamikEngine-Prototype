#include "mngafx.h"
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
			if (file.is_open())
				file.close();
			file.open(path);
		}

		bool daiManager::isOpen() {
			return file.is_open();
		}

		std::string daiManager::getLine() {
			std::string line = DMK_TEXT("");
			//std::getline(file, line, '\n');
			file.getline(line.data(), '\n');

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

		ARRAY<std::string> daiManager::getData(DMKDaiFileDataType type) {
			if (!isDataStoreInit)
				initDataStore();

			if (type > DMK_DAI_FILE_DATA_TYPE_CODE)
				return ARRAY<std::string>();

			return dataStore[type];
		}

		ARRAY<ARRAY<std::string>> daiManager::getAllData() {
			if (!isDataStoreInit)
				initDataStore();

			return dataStore;
		}

		ARRAY<float> daiManager::getLocationData() {
			if (!isDataStoreInit)
				initDataStore();

			return locationData;
		}

		ARRAY<float> daiManager::getRotationData() {
			if (!isDataStoreInit)
				initDataStore();

			return rotationData;
		}

		void daiManager::initDataStore() {
			dataStore.resize(DMK_DAI_FILE_DATA_TYPE_MAX_ENUM);
			locationData.resize(3);
			rotationData.resize(3);

			std::string _line = DMK_TEXT("");
			while (getLine(&_line)) {
				if (_line[0] == 'M' || _line[0] == 'm')	// Models
					dataStore[DMK_DAI_FILE_DATA_TYPE_MODEL].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'I' || _line[0] == 'i')	// Textures
					dataStore[DMK_DAI_FILE_DATA_TYPE_TEXTURE].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'V' || _line[0] == 'v')	// Vertex shader
					dataStore[DMK_DAI_FILE_DATA_TYPE_VERTEX].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'T' || _line[0] == 't')	// Tessellation shader
					dataStore[DMK_DAI_FILE_DATA_TYPE_TESSELLATION].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'G' || _line[0] == 'g')	// Geometry shader
					dataStore[DMK_DAI_FILE_DATA_TYPE_GEOMETRY].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'F' || _line[0] == 'f')	// Fragment shader
					dataStore[DMK_DAI_FILE_DATA_TYPE_FRAGMENT].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'X' || _line[0] == 'x')	// Shader source file
					dataStore[DMK_DAI_FILE_DATA_TYPE_SHADER_SOURCE].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'S' || _line[0] == 's')	// Script file
					dataStore[DMK_DAI_FILE_DATA_TYPE_SCRIPT].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'C' || _line[0] == 'c')	// Code file
					dataStore[DMK_DAI_FILE_DATA_TYPE_CODE].push_back((_line.substr(2, _line.size() - 2)));
				else if (_line[0] == 'L' || _line[0] == 'l') {	// Location data
					if (_line[1] == 'X' || _line[1] == 'x')	// X-Axis
						locationData[0] = std::stof(_line.substr(3, _line.size() - 3));
					if (_line[1] == 'Y' || _line[1] == 'y')	// Y-Axis
						locationData[1] = std::stof(_line.substr(3, _line.size() - 3));
					if (_line[1] == 'Z' || _line[1] == 'z')	// Z-Axis
						locationData[2] = std::stof(_line.substr(3, _line.size() - 3));
				}
				else if (_line[0] == 'R' || _line[0] == 'r') {	// Rotation data
					if (_line[1] == 'X' || _line[1] == 'x')	// X-Axis
						rotationData[0] = std::stof(_line.substr(3, _line.size() - 3));
					if (_line[1] == 'Y' || _line[1] == 'y')	// Y-Axis
						rotationData[1] = std::stof(_line.substr(3, _line.size() - 3));
					if (_line[1] == 'Z' || _line[1] == 'z')	// Z-Axis
						rotationData[2] = std::stof(_line.substr(3, _line.size() - 3));
				}
			}

			file.clear();
			file.seekg(0, std::ios::beg);

			isDataStoreInit = true;
		}
	}
}
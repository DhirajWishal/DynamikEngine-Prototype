#pragma once

#ifndef _DYNAMIK_MANAGERS_DAI_FILE_MANAGER_H
#define _DYNAMIK_MANAGERS_DAI_FILE_MANAGER_H

#include <string>
#include <fstream>

namespace Dynamik {
	namespace utils {
		enum DMKDaiFileDataType {
			DMK_DAI_FILE_DATA_TYPE_MODEL,
			DMK_DAI_FILE_DATA_TYPE_TEXTURE,
			DMK_DAI_FILE_DATA_TYPE_VERTEX,
			DMK_DAI_FILE_DATA_TYPE_TESSELLATION,
			DMK_DAI_FILE_DATA_TYPE_GEOMETRY,
			DMK_DAI_FILE_DATA_TYPE_FRAGMENT,
			DMK_DAI_FILE_DATA_TYPE_SCRIPT,
		};

		class daiManager {
		public:
			daiManager();
			daiManager(std::string& path);
			~daiManager();

			void open(std::string path);

			bool isOpen();
			std::string getLine();
			std::istream& getLine(std::string*);

			std::fstream& getFile();

			void close();

			std::vector<std::string> getData(DMKDaiFileDataType type);
			std::vector<std::vector<std::string>> getAllData();

		private:
			void initDataStore();

			std::string myPath = "";
			std::fstream file;

			std::vector<std::vector<std::string>> dataStore = {};
			bool isDataStoreInit = false;
		};
	}
}

#endif

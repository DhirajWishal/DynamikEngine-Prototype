#pragma once

/*
 Dynamik Asset Index (DAI) file handler

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		03/12/2019
 IDE:		MS Visual Studio Community 2019

 daiFileManager.h file

 All assets must contain a Dynamik Asset Index file to index that perticular asset.
 A separate file can be used to index all DAI files for a perticular level. Each
 DAI file supports a variety of assets,
	* Model(s) (M/ m)
	* Texture(s) (I/ i)
	* Shaders
		- Vertex (V/ v)
		- Tessellation (T/ t)
		- Geometry (G/ g)
		- Fragment (F/ f)
		- Raw shader file(s) (X/ x)
	* Script(s) (S/ s)
	* Code(s) (C/ c)
	* Location data (in world space)
		- X-axis (LX)
		- Y-axis (LY)
		- Z-axis (LZ)
	* Rotational data (in world space)
		- X-axis (RX)
		- Y-axis (RY)
		- Z-axis (RZ)
*/

#ifndef _DYNAMIK_MANAGERS_DAI_FILE_MANAGER_H
#define _DYNAMIK_MANAGERS_DAI_FILE_MANAGER_H

#include <string>
#include <fstream>
#include <vector>

#include "Platform.h"

namespace Dynamik {
	namespace utils {
		enum DMKDaiFileDataType {
			DMK_DAI_FILE_DATA_TYPE_MODEL,
			DMK_DAI_FILE_DATA_TYPE_TEXTURE,
			DMK_DAI_FILE_DATA_TYPE_VERTEX,
			DMK_DAI_FILE_DATA_TYPE_TESSELLATION,
			DMK_DAI_FILE_DATA_TYPE_GEOMETRY,
			DMK_DAI_FILE_DATA_TYPE_FRAGMENT,
			DMK_DAI_FILE_DATA_TYPE_SHADER_SOURCE,
			DMK_DAI_FILE_DATA_TYPE_SCRIPT,
			DMK_DAI_FILE_DATA_TYPE_CODE,
			DMK_DAI_FILE_DATA_TYPE_LOCATION,
			DMK_DAI_FILE_DATA_TYPE_ROTATION,

			DMK_DAI_FILE_DATA_TYPE_MAX_ENUM
		};

		class DMK_API daiManager {
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
			std::vector<float> getLocationData();
			std::vector<float> getRotationData();

		private:
			void initDataStore();

			std::string myPath = "";
			std::fstream file = {};

			std::vector<std::vector<std::string>> dataStore = {};
			std::vector<float> locationData = {};
			std::vector<float> rotationData = {};
			bool isDataStoreInit = false;
		};
	}
}

#endif

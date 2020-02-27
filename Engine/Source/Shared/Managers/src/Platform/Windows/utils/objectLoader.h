#pragma once

/*
 3D object loader for Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		22/12/2019
 IDE:		MS Visual Studio Community 2019

 objectLoader.h file
*/

#ifndef _DYNAMIK_OBJECT_LOADER_H
#define _DYNAMIK_OBJECT_LOADER_H

#include <string>
#include <fstream>
#include <vector>

#include "Platform.h"
#include "core/data store/internalFormat.h"

namespace Dynamik {
	namespace utils {
		// Object data container 
		struct DMK_API DMKObjectDataContainer {
			std::vector<std::vector<float>> myVertices = {};
			std::vector<std::vector<float>> myTextureCoordinates = {};
			std::vector<std::vector<float>> myNormals = {};
			std::vector<std::vector<float>> myParameterSpaces = {};
			std::vector<std::vector<uint32_t>> myLineElements = {};
			std::vector<std::string> myMTLFile = {};

			std::vector<uint32_t> myVertexIndex = {};
			std::vector<uint32_t> myTextureCoordinateIndex = {};
			std::vector<uint32_t> myNormalIndex = {};
			uint32_t myIndexCount = 0;
		};

		// obj file loader
		//std::vector<DMKObjectDataContainer> loadObjFileData(std::string_view path);
		DMK_API void loadObjFileData(InternalFormat* myInternalFormat = nullptr);
	}
}

#endif

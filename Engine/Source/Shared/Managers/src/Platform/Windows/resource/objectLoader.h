#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_OBJECT_LOADER_H
#define _DYNAMIK_MANAGERS_WINDOWS_OBJECT_LOADER_H

#include <string>
#include <fstream>
#include <vector>

#include "Platform.h"
#include "core/data store/internalFormat.h"

namespace Dynamik {
	namespace resources {
		//struct ObjectData {
		//	tinyobj::attrib_t attributes;
		//	std::vector<tinyobj::shape_t> shapes;
		//	std::vector<tinyobj::material_t> materials;
		//	std::string warnings, errors;
		//
		//	bool loadObject(std::string path);
		//};

		template<typename Alloc>
		void loadObject(std::string& path, std::vector<Alloc>* vertices, std::vector<uint32_t>* indices);
	}
	namespace utils {
		// Object data container
		struct  DMKObjectDataContainer {
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
		 void loadObjFileData(InternalFormat* myInternalFormat = nullptr);
	}
}

#endif // !_DYNAMIK_MANAGERS_OBJECT_LOADER_H

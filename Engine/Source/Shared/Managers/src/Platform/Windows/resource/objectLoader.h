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
		//	ARRAY<tinyobj::shape_t> shapes;
		//	ARRAY<tinyobj::material_t> materials;
		//	std::string warnings, errors;
		//
		//	bool loadObject(std::string path);
		//};

		template<typename Alloc>
		void loadObject(std::string& path, ARRAY<Alloc>* vertices, ARRAY<uint32_t>* indices);
	}
	namespace utils {
		// Object data container
		struct  DMKObjectDataContainer {
			ARRAY<ARRAY<float>> myVertices = {};
			ARRAY<ARRAY<float>> myTextureCoordinates = {};
			ARRAY<ARRAY<float>> myNormals = {};
			ARRAY<ARRAY<float>> myParameterSpaces = {};
			ARRAY<ARRAY<uint32_t>> myLineElements = {};
			ARRAY<std::string> myMTLFile = {};

			ARRAY<uint32_t> myVertexIndex = {};
			ARRAY<uint32_t> myTextureCoordinateIndex = {};
			ARRAY<uint32_t> myNormalIndex = {};
			uint32_t myIndexCount = 0;
		};

		// obj file loader
		//ARRAY<DMKObjectDataContainer> loadObjFileData(std::string_view path);
		void loadObjFileData(InternalFormat* myInternalFormat = nullptr);
	}
}

#endif // !_DYNAMIK_MANAGERS_OBJECT_LOADER_H

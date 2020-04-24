#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_OBJECT_LOADER_H
#define _DYNAMIK_MANAGERS_WINDOWS_OBJECT_LOADER_H

#include <string>
#include <fstream>
#include <vector>

#include "Public/Array.h"
#include "Objects/InternalFormat/InternalFormat.h"

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
		void loadObject(std::string& path, ARRAY<Alloc>* vertices, ARRAY<UI32>* indices);
	}
	namespace utils {
		// Object data container
		struct  DMKObjectDataContainer {
			ARRAY<ARRAY<float>> myVertices = {};
			ARRAY<ARRAY<float>> myTextureCoordinates = {};
			ARRAY<ARRAY<float>> myNormals = {};
			ARRAY<ARRAY<float>> myParameterSpaces = {};
			ARRAY<ARRAY<UI32>> myLineElements = {};
			ARRAY<std::string> myMTLFile = {};

			ARRAY<UI32> myVertexIndex = {};
			ARRAY<UI32> myTextureCoordinateIndex = {};
			ARRAY<UI32> myNormalIndex = {};
			UI32 myIndexCount = 0;
		};

		// obj file loader
		//ARRAY<DMKObjectDataContainer> loadObjFileData(std::string_view path);
		void loadObjFileData(POINTER<InternalFormat> myInternalFormat);
	}
}

#endif // !_DYNAMIK_MANAGERS_OBJECT_LOADER_H

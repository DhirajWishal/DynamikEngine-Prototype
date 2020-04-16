#pragma once

#ifndef _DYNAMIK_AGDR_VULKAN_INTERNAL_FORMAT_H
#define _DYNAMIK_AGDR_VULKAN_INTERNAL_FORMAT_H

#include "internalObjectFormat.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "CentralDataHub.h"

namespace Dynamik {
	class InternalFormat {
	public:
		InternalFormat(GameObject* object) : myGameObject(object) {}
		virtual ~InternalFormat() {}

		GameObject* myGameObject = nullptr;

		void initVertedAndIndexBufferObjects(int count) {
			myVertexBufferObjects.resize(count);
			myIndexBufferObjects.resize(count);
		}

		// Paths
		ARRAY<std::string> myModelPaths = {};
		ARRAY<std::string> myTexturePaths = {};
		ARRAY<std::string> myShaderPaths = {};

		// Textures
		UI32 myMipLevel = 1.0f;

		// Vertex buffers
		ARRAY<ARRAY<Vertex>> myVertexBufferObjects = {};
		ARRAY<UI32> myVertexCounts;

		// Index buffers
		ARRAY<ARRAY<UI32>> myIndexBufferObjects = {};
		ARRAY<UI32> myIndexCounts;

		bool isInitializedPrimary = false;
	};
}

#endif

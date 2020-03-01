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
		std::vector<std::wstring> myModelPaths = {};
		std::vector<std::wstring> myTexturePaths = {};
		std::vector<std::wstring> myShaderPaths = {};

		// Textures
		uint32_t myMipLevel = 1.0f;

		// Vertex buffers
		std::vector<std::vector<Vertex>> myVertexBufferObjects = {};
		std::vector<uint32_t> myVertexCounts;

		// Index buffers
		std::vector<std::vector<uint32_t>> myIndexBufferObjects = {};
		std::vector<uint32_t> myIndexCounts;

		bool isInitializedPrimary = false;
	};
}

#endif

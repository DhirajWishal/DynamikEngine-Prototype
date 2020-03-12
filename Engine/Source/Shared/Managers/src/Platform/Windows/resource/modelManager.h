#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H

#include "core/renderer/vertex.h"
#include "core/core.h"

#include "Platform.h"

namespace Dynamik {
	struct  DMKModelLoadInfo {
		std::string path = "";

		std::vector<Vertex>* vertices = {};
		std::vector<uint32_t>* indices = {};

		std::vector<float> vertexOffset = { 0.0f, 0.0f, 0.0f };	// x, y, z
	};

	 void loadModel(DMKModelLoadInfo info);
}

#endif	// !_DYNAMIK_ADGR_VULKAN_MODEL_MANAGER_H

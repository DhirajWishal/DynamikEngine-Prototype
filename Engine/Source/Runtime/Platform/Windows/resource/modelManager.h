#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H

#include "core.h"
#include "Objects/Mesh/Mesh.h"

namespace Dynamik {
	struct  DMKModelLoadInfo {
		std::string path = "";
		ARRAY<Mesh>* meshes;
		ARRAY<float> vertexOffset = { 0.0f, 0.0f, 0.0f };	// x, y, z
	};

	void loadModel(DMKModelLoadInfo info);
};

#endif	// !_DYNAMIK_ADGR_VULKAN_MODEL_MANAGER_H

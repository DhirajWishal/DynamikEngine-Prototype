#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H

#include "core.h"
#include "Objects/Mesh/Mesh.h"

namespace Dynamik {
	struct  DMKAssetLoadInfo {
		/* Object info */
		std::string path = "";
		ARRAY<Mesh>* meshes;
		ARRAY<float> vertexOffset = { 0.0f, 0.0f, 0.0f };	// x, y, z

		/* Texture info */
		DMKTextureType textureType = DMKTextureType::DMK_TEXTURE_TYPE_2D;
		ARRAY<std::string> texturePaths;
		DMKTextureInputType textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_AUTO;
	};

	void loadModel(DMKAssetLoadInfo info);
};

#endif	// !_DYNAMIK_ADGR_VULKAN_MODEL_MANAGER_H

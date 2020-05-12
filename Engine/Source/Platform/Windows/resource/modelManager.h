#pragma once

#ifndef _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H
#define _DYNAMIK_MANAGERS_WINDOWS_MODEL_MANAGER_H


#include "Objects/Mesh/Mesh.h"

namespace Dynamik {
	struct  DMKAssetLoadInfo {
		/* Object info */
		std::string path = "";
		std::vector<Mesh>* meshes;
		std::vector<float> vertexOffset = { 0.0f, 0.0f, 0.0f };	// x, y, z

		/* Texture info */
		DMKTextureType textureType = DMKTextureType::DMK_TEXTURE_TYPE_2D;
		std::vector<std::string> texturePaths;
		DMKTextureInputType textureInputType = DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_AUTO;
	};

	void loadModel(DMKAssetLoadInfo info);
};

#endif	// !_DYNAMIK_RENDERER_VULKAN_MODEL_MANAGER_H

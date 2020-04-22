#pragma once
#ifndef _DYNAMIK_INTERNAL_FORMAT_H
#define _DYNAMIK_INTERNAL_FORMAT_H

#include "GameObject.h"
#include "../Texture/Texture.h"
#include "../Mesh/Mesh.h"

namespace Dynamik {
	struct ShaderPaths {
		std::string vertexShader = "";
		std::string tessellationShader = "";
		std::string geometryShader = "";
		std::string fragmentShader = "";
	};

	/* INTERNAL GAME OBJECT FORMAT
	 * This consists the data required by the engine.
	 */
	class InternalFormat : public DMKGameObject {
	public:
		InternalFormat() {}
		~InternalFormat() {}

		/* Object meshes */
		ARRAY<Mesh> meshDatas;

		/* Texture data store */
		ARRAY<Texture> textures;

		/* Object data paths */
		CCPTR objectPath = "";
		ARRAY<CCPTR> texturePaths;
		ShaderPaths shaderPaths;

		/* Uniform buffer data */
		alignas (16) ARRAY<MAT4> uniformBufferData;
	};
}

#endif // !_DYNAMIK_INTERNAL_FORMAT_H
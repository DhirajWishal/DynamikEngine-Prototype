#pragma once
#ifndef _DYNAMIK_INTERNAL_FORMAT_H
#define _DYNAMIK_INTERNAL_FORMAT_H

#include "GameObject.h"
#include "../Texture/Texture.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/AnimatedMesh.h"

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
		InternalFormat();
		virtual ~InternalFormat() {}

		B1 checkRayCollition(VEC3 rayOrigin, VEC3 rayDirection, MeshVertexLimits limits);
		B1 isSelected = false;

		/* Object meshes */
		std::vector<Mesh> meshDatas;
		std::vector<AnimatedMesh> animation;

		/* Object data paths */
		std::string objectPath = "";
		std::vector<std::string> texturePaths;
		ShaderPaths shaderPaths;
	};
}

#endif // !_DYNAMIK_INTERNAL_FORMAT_H

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

	/* Directly translated to descriptors and objects */
	enum class DMKRenderAttachment {
		DMK_RENDER_ATTACHMENT_SKYBOX,
		DMK_RENDER_ATTACHMENT_LIGHTING,
		DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE,
		DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE,
		DMK_RENDER_ATTACHMENT_BRDF_TABLE,

		DMK_RENDER_ATTACHMENT_COMPUTE_TEXTURE,
	};

	/* INTERNAL GAME OBJECT FORMAT
	 * This consists the data required by the engine.
	 */
	class InternalFormat : public DMKGameObject {
	public:
		InternalFormat();
		virtual ~InternalFormat() {}

		/* Object meshes */
		ARRAY<Mesh> meshDatas;

		/* Texture data store */
		ARRAY<Texture> textures;

		/* Object data paths */
		std::string objectPath = "";
		ARRAY<std::string> texturePaths;
		ShaderPaths shaderPaths;

		/* Uniform buffer data */
		alignas (16) ARRAY<MAT4> uniformBufferData;

		/* Filled by the Renderer */
	public:
		ARRAY<DMKRenderAttachment> renderAttachments;
	};
}

#endif // !_DYNAMIK_INTERNAL_FORMAT_H

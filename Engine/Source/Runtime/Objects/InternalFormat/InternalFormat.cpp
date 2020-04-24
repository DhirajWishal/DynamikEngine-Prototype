#include "dmkafx.h"
#include "InternalFormat.h"

namespace Dynamik {
	InternalFormat::InternalFormat() : DMKGameObject()
	{
		meshDatas = ARRAY<Mesh>();
		textures = ARRAY<Texture>();

		objectPath = std::string();
		texturePaths = ARRAY<std::string>();
		shaderPaths = ShaderPaths();

		uniformBufferData = ARRAY<MAT4>();
	}
}
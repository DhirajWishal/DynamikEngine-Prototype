#include "dmkafx.h"
#include "InternalFormat.h"

namespace Dynamik {
	InternalFormat::InternalFormat() : DMKGameObject()
	{
		meshDatas = ARRAY<Mesh>();

		objectPath = std::string();
		texturePaths = ARRAY<std::string>();
		shaderPaths = ShaderPaths();
	}
}
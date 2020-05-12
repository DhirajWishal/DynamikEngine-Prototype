#include "dmkafx.h"
#include "InternalFormat.h"

namespace Dynamik {
	InternalFormat::InternalFormat() : DMKGameObject()
	{
		meshDatas = std::vector<Mesh>();

		objectPath = std::string();
		texturePaths = std::vector<std::string>();
		shaderPaths = ShaderPaths();
	}
}
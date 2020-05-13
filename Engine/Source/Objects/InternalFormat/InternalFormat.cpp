#include "dmkafx.h"
#include "InternalFormat.h"

#include <glm/gtx/intersect.hpp>

namespace Dynamik {
	InternalFormat::InternalFormat() : DMKGameObject()
	{
		meshDatas = std::vector<Mesh>();

		objectPath = std::string();
		texturePaths = std::vector<std::string>();
		shaderPaths = ShaderPaths();
	}

	B1 InternalFormat::checkRayCollition(VEC3 rayOrigin, VEC3 rayDirection)
	{
		VEC3 v = descriptor.transformDescriptor.location - rayDirection;
		D64 a = glm::dot(rayDirection, rayDirection);
		D64 b = 2.0 * glm::dot(v, rayDirection);
		D64 c = glm::dot(v, v) - descriptor.transformDescriptor.hitBoxRadius * descriptor.transformDescriptor.hitBoxRadius;
		D64 b_squared_minus_4ac = b * b + (-4.0) * a * c;

		if (b_squared_minus_4ac > 0)
		{
			// Herein lies the problem
			D64 x1 = (-b - sqrt(b_squared_minus_4ac)) / (2.0 * a);
			D64 x2 = (-b + sqrt(b_squared_minus_4ac)) / (2.0 * a);

			// Neither test true from far away
			if (x1 >= 0.0 && x2 >= 0.0)
			{
				DMK_CORE_INFO("Ray collided with object! " + std::to_string((UI32)type));
				return true;
			}
			if (x1 < 0.0 && x2 >= 0.0)
			{
				//DMK_CORE_WARN("Ray collided with object! " + std::to_string((UI32)type));
				return true;
			}
		}

		return false;

	}
}
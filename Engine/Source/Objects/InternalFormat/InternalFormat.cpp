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

	B1 InternalFormat::checkRayCollition(VEC3 rayOrigin, VEC3 rayDirection, MeshVertexLimits limits)
	{
		F32 tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (limits.limitX.x - rayOrigin.x) / rayDirection.x;
		tmax = (limits.limitX.y - rayOrigin.x) / rayDirection.x;
		tymin = (limits.limitY.x - rayOrigin.y) / rayDirection.y;
		tymax = (limits.limitY.y - rayOrigin.y) / rayDirection.y;

		if ((tmin > tymax) || (tymin > tmax))
		{
			isSelected = false;
			return false;
		}

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (limits.limitZ.x - rayOrigin.z) / rayDirection.z;
		tzmax = (limits.limitZ.y - rayOrigin.z) / rayDirection.z;

		if ((tmin > tzmax) || (tzmin > tmax))
		{
			isSelected = false;
			return false;
		}

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		isSelected = true;
		return true;
	}
}
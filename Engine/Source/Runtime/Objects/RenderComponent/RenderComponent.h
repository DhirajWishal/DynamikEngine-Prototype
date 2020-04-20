#pragma once
#ifndef _DYNAMIK_RENDER_COMPONENT_H
#define _DYNAMIK_RENDER_COMPONENT_H

#include "../Texture/Texture.h"
#include "Public/Array.h"
#include "../IndexBufferObject/IndexBufferObject.h"
#include "../VertexBufferObject/VertexBufferObject.h"
#include "../UniformBufferObject/UniformBufferObject.h"

namespace Dynamik {
	class RenderComponent : public DMKObject {
	public:
		RenderComponent() {}
		~RenderComponent() {}

		ARRAY<VertexBufferObject*> vertexBufferObjects;
		ARRAY<IndexBufferObject*> indexBufferObjects;
		ARRAY<Texture> textures;
		ARRAY<UniformBufferObject> uniformBufferObjects;
	};
}

#endif // !_DYNAMIK_RENDER_COMPONENT_H

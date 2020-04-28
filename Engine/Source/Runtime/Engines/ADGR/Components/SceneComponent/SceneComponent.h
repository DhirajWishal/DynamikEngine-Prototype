#pragma once
#ifndef _DYNAMIK_ADGR_SCENE_COMPONENT_H
#define _DYNAMIK_ADGR_SCENE_COMPONENT_H

#include "Objects/InternalFormat/InternalFormat.h"

#include "../../Backend/Primitives/IndexBuffer.h"
#include "../../Backend/Primitives/VertexBuffer.h"
#include "../../Backend/Primitives/TextureData.h"
#include "../../Backend/Primitives/UniformBuffer.h"
#include "../../Backend/Primitives/Pipeline.h"

namespace Dynamik {
	namespace ADGR {
		struct RenderingMeshComponent {
			POINTER<VertexBuffer> vertexBuffer;
			POINTER<IndexBuffer> indexBuffer;
			POINTER<TextureData> textureData;
			ARRAY<POINTER<UniformBuffer>> uniformBuffers;
		};

		class SceneComponent {
		public:
			SceneComponent() {}
			virtual ~SceneComponent() {}

		protected:
			POINTER<InternalFormat> myInternalFormat;
			ARRAY<RenderingMeshComponent> myMeshes;
		};
	}
}

#endif // !_DYNAMIK_ADGR_SCENE_COMPONENT_H

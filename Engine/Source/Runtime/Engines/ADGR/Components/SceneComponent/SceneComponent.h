#pragma once
#ifndef _DYNAMIK_ADGR_SCENE_COMPONENT_H
#define _DYNAMIK_ADGR_SCENE_COMPONENT_H

#include "Objects/InternalFormat/InternalFormat.h"

#include "../../Backend/Primitives/IndexBuffer.h"
#include "../../Backend/Primitives/VertexBuffer.h"
#include "../../Backend/Primitives/TextureData.h"
#include "../../Backend/Primitives/UniformBuffer.h"
#include "../../Backend/Primitives/Pipeline.h"

//#include "../../Backend/Vulkan/VulkanRBL.h"

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

			virtual void initializeResources() {}
			virtual void initializeContext() {}
			virtual DMKUniformBufferData update(DMKCameraData cameraData) { return DMKUniformBufferData(); }
			virtual void terminate() {}

			POINTER<InternalFormat> myInternalFormat;
			ARRAY<RenderingMeshComponent> myMeshes;
			//POINTER<RenderContext> myRenderContext;
			POINTER<Pipeline> myPipeline;

			DMKRenderingTechnology renderingTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;
		};
	}
}

#endif // !_DYNAMIK_ADGR_SCENE_COMPONENT_H

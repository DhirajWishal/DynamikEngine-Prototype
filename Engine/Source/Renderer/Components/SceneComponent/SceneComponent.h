#pragma once
#ifndef _DYNAMIK_RENDERER_SCENE_COMPONENT_H
#define _DYNAMIK_RENDERER_SCENE_COMPONENT_H

#include "Objects/InternalFormat/InternalFormat.h"

#include "../../Backend/Primitives/IndexBuffer.h"
#include "../../Backend/Primitives/VertexBuffer.h"
#include "../../Backend/Primitives/TextureData.h"
#include "../../Backend/Primitives/UniformBuffer.h"
#include "../../Backend/Primitives/Pipeline.h"

//#include "../../Backend/Vulkan/VulkanRBL.h"

namespace Dynamik {
	namespace Renderer {
		struct RenderingMeshComponent {
			POINTER<VertexBuffer> vertexBuffer;
			POINTER<IndexBuffer> indexBuffer;
			POINTER<TextureData> textureData;
			std::vector<POINTER<UniformBuffer>> uniformBuffers;
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
			std::vector<RenderingMeshComponent> myMeshes;
			//POINTER<RenderContext> myRenderContext;
			POINTER<Pipeline> myPipeline;

			DMKRenderingTechnology renderingTechnology = DMKRenderingTechnology::DMK_RENDERING_TECHNOLOGY_INDEXED;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_SCENE_COMPONENT_H

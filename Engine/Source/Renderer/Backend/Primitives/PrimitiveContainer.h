#pragma once
#ifndef _DYNAMIK_RENDERER_PRIMITIVE_CONTAINER_H
#define _DYNAMIK_RENDERER_PRIMITIVE_CONTAINER_H

#include "FrameBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "TextureData.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"

#include "../../Components/SceneComponent/SceneComponent.h"

namespace Dynamik {
	namespace Renderer {
		enum class RenderContextType {
			RENDER_CONTEXT_TYPE_2D,
			RENDER_CONTEXT_TYPE_3D,
			RENDER_CONTEXT_TYPE_DEFAULT,
			RENDER_CONTEXT_TYPE_OVERLAY,
		};

		/* Renderable Mesh Container
		 * This container stores all the data necessary to render a mesh.
		 * It containes vertex buffers, index buffers and textures.
		 */
		struct RenderableMeshContainer {
			POINTER<VertexBuffer> vertexBuffer;
			POINTER<IndexBuffer> indexBuffer;
			POINTER<TextureData> textureData;
		};

		/* Render Context
		 * Render context is an instance of render where a set of objects are rendered.
		 * An rendering instance can have multiple render contexts.
		 * A render context can have multiple render objects while a renderable object has one render context.
		 */
		struct RenderContext {
			POINTER<SwapChain> swapChain;
			POINTER<RenderPass> renderPass;
			ARRAY<POINTER<FrameBuffer>> frameBuffers;

			ARRAY<POINTER<SceneComponent>> sceneComponents;

			RenderContextType type = RenderContextType::RENDER_CONTEXT_TYPE_DEFAULT;
		};

		/* Renderable Uniform Buffer Container
		 * This contains a set of uniform buffers which contains the Uniform Buffer Primitives.
		 * This buffer is updated with the Uniform Buffer Object associated to it on draw time.
		 */
		struct RenderableUniformBufferContainer {
			ARRAY<POINTER<UniformBuffer>> uniformBuffers;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_PRIMITIVE_CONTAINER_H

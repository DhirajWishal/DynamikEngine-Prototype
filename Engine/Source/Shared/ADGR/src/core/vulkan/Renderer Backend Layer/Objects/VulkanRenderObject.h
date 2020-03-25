#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDER_OBJECT_H
#define _DYNAMIK_ADGR_VULKAN_RENDER_OBJECT_H

#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanTexture.h"
#include "VulkanUniformBuffer.h"
#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			class VulkanDescriptors;
			class VulkanUniformBuffer;
			class VulkanPipeline;

			class VulkanVertexBuffer;
			class VulkanIndexBuffer;
			class VulkanTexture;
			class VulkanPushConstant;

			class VulkanRenderObject {
			public:
				VulkanRenderObject() {}
				~VulkanRenderObject() {}

				DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;

				VulkanRenderPass renderPass;
				VulkanPipeline pipeline;

				ARRAY<VulkanVertexBuffer> vertexBuffers;
				ARRAY<VulkanIndexBuffer> indexBuffers;
				ARRAY<VulkanTexture> textures;
				ARRAY<VulkanPushConstant> pushConstants;

				VulkanUniformBuffer uniformBuffer;
				VulkanDescriptors descriptorSet;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDER_OBJECTS_CONTAINER_H

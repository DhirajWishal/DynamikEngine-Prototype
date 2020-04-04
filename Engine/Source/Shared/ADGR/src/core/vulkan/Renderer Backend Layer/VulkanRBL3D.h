#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H

#include "Objects/VulkanRBLIndex.h"
#include "core/base/RendererBackendBase.h"
#include "core/Window/Windows/WindowManager.h"

#include "core/Components/Camera3D.h"

namespace Dynamik {
	namespace ADGR {
		using namespace Backend;

		struct ADGRVulkan3DObjectData {
			POINTER<ARRAY<ARRAY<Vertex>>> vertexBufferObjects;
			POINTER<ARRAY<ARRAY<UI32>>> indexBufferObjects;

			std::string vertexShaderPath = "";
			std::string tessellationShaderPath = "";
			std::string geometryShaderPath = "";
			std::string fragmentShaderPath = "";

			ARRAY<std::string> texturePaths;

			DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
		};

		class VulkanRBL3D : public RendererBackend {
		public:
			VulkanRBL3D() {}
			~VulkanRBL3D() {}

			void init() override;
			void initStageOne() override;
			void initStageTwo() override;
			void initStageThree() override;

			void drawFrame() override;
			void recreateSwapChain();

			void shutDown() override;
			void shutDownStageOne() override;
			void shutDownStageTwo() override;
			void shutDownStageThree() override;

			void getObjects(ARRAY<ADGRVulkan3DObjectData> objectDatas);
			void update();

			std::deque<DMKEventContainer>* events() override;

		private:
			void initializeObjects();

			Camera3D myCamera;

			WindowManager myWindowManager;
			std::deque<DMKEventContainer> eventsContainer;

			VulkanCore myVulkanCore;
			VulkanColorBuffer myColorBuffer;
			VulkanDepthBuffer myDepthBuffer;

			ARRAY<VulkanRenderableObject> renderableObjects;
			ARRAY<ADGRVulkan3DObjectData> rawObjectStore;

			UI32 imageIndex = 0;
			UI32 currentFrame = 0;

			VkResult result;
			VkSubmitInfo submitInfo = {};
			VkPresentInfoKHR presentInfo = {};
			VkSemaphore waitSemaphores[1];
			VkPipelineStageFlags waitStages[1];
			VkSemaphore signalSemaphores[1];
			VkSwapchainKHR swapChains[1];
		};
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H

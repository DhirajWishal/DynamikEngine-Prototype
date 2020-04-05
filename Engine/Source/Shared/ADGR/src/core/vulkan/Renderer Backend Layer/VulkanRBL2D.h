#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_2D_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_2D_H

#include <GLFW/glfw3.h>

#include "Objects/VulkanRBLIndex.h"
#include "core/base/RendererBackendBase.h"
#include "core/Window/Windows/WindowManager.h"
#include "core/Components/Camera2D.h"

namespace Dynamik {
	namespace ADGR {
		using namespace Backend;

		struct ADGRVulkan2DObjectData {
			POINTER<ARRAY<ARRAY<vertex2D>>> vertexBufferObjects;
			POINTER<ARRAY<ARRAY<UI32>>> indexBufferObjects;

			std::string vertexShaderPath = "";
			std::string tessellationShaderPath = "";
			std::string geometryShaderPath = "";
			std::string fragmentShaderPath = "";

			ARRAY<std::string> texturePaths;

			DMK_ADGR_RENDERING_TECHNOLOGY renderTechnology = DMK_ADGR_RENDERING_TECHNOLOGY::DMK_ADGR_RENDER_INDEXED;
		};

		class VulkanRBL2D : public RendererBackend {
		public:
			VulkanRBL2D() {}
			~VulkanRBL2D() {}

			void setWindowHandle(GLFWWindowHandleContainer container) { myWindowHandle = container; }

			void init() override;
			void initStageOne() override;
			void initStageTwo() override;
			void initStageThree() override;

			void drawFrame(std::deque<DMKEventContainer> container) override;
			void recreateSwapChain();

			void shutDown() override;
			void shutDownStageOne() override;
			void shutDownStageTwo() override;
			void shutDownStageThree() override;

			void getObjects(ARRAY<ADGRVulkan2DObjectData> objectDatas);
			void update();

		private:
			void initializeObjects();
			void initializeRenderPass();

			Camera2D myCamera;
			GLFWWindowHandleContainer myWindowHandle;
			VulkanCore myVulkanCore;

			ARRAY<VulkanRenderableObject> renderableObjects;
			ARRAY<ADGRVulkan2DObjectData> rawObjectStore;

			UI32 imageIndex = 0;
			UI32 currentFrame = 0;

			VkResult result;
		};
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RBL_2D_H

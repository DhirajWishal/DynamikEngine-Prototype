#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H

#include "Managers/VulkanRBLIndex.h"
#include "core/base/RendererBackendBase.h"
#include "core/Window/Windows/WindowManager.h"

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

			void shutDown() override;
			void shutDownStageOne() override;
			void shutDownStageTwo() override;
			void shutDownStageThree() override;

			void getObjects(ARRAY<ADGRVulkan3DObjectData> objectDatas);
			void update();

		private:
			void initializeObjects();

			WindowManager myWindowManager;

			VulkanQueueManager myQueue;
			VulkanCoreObject myCoreObject;

			ARRAY<VulkanRenderObject> renderableObjects;
			ARRAY<VulkanRenderObject> internalObjects;

			ARRAY<ADGRVulkan3DObjectData> rawObjectStore;

			ADGRVulkanSyncObjectsContainer syncObjectsContainer;

			VkDevice device;
		};
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H

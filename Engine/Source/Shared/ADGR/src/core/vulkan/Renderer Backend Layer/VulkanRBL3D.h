#pragma once
#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H

#include "Objects/VulkanRBLIndex.h"
#include "core/base/RendererBackendBase.h"
#include "core/Window/Windows/WindowManager.h"

namespace Dynamik {
	namespace ADGR {
		using namespace Backend;

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

		private:
			WindowManager myWindowManager;

			VulkanInstance myInstance;
			VulkanDebugger myDebugger;
			VulkanDevice myDevice;
			VulkanSwapChain mySwapChain;
			VulkanCommandBuffer myCommandBuffer;
			VulkanColorBuffer myColorBuffer;
			VulkanDepthBuffer myDepthBuffer;
			VulkanRenderPass myRenderPass;
			VulkanFrameBuffer myFrameBuffer;

			VulkanQueue myQueue;
			VulkanGlobalVariables myGlobals;

			VulkanSyncObjects mySyncObjects;

			ARRAY<VulkanRenderObject> renderableObjects;
			ARRAY<VulkanRenderObject> internalObjects;
		};
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_BACKEND_LAYER_3D_H

#include "adgrafx.h"
#include "VulkanRBL3D.h"

namespace Dynamik {
	namespace ADGR {
		void VulkanRBL3D::init()
		{
			initStageOne();
			initStageTwo();
			initStageThree();
		}
		
		void VulkanRBL3D::initStageOne()
		{
			ADGRVulkanInstanceInitInfo instanceInitInfo;
			instanceInitInfo.applicationName = "Dynamik Engine";
			instanceInitInfo.engineName = "Dynamik";
			myInstance.initialize(instanceInitInfo);

			myDebugger.initialize(myInstance);

			// TODO: surface initialization

			myDevice.initialize(myInstance, &myQueue, &myGlobals);

			mySwapChain.initialize(myInstance, myDevice, 720, 480);	// TODO::

			myCommandBuffer.initializeCommandPool(myInstance, myDevice);

			ADGRVulkanColorBufferInitInfo colorBufferInitInfo;
			colorBufferInitInfo.commandBuffer = myCommandBuffer;
			colorBufferInitInfo.device = myDevice;
			colorBufferInitInfo.global = myGlobals;
			colorBufferInitInfo.queue = myQueue;
			colorBufferInitInfo.swapChain = mySwapChain;
			myColorBuffer.initialize(colorBufferInitInfo);

			ADGRVulkanDepthBufferInitInfo depthBufferInitInfo;
			depthBufferInitInfo.commandBuffer = myCommandBuffer;
			depthBufferInitInfo.device = myDevice;
			depthBufferInitInfo.global = myGlobals;
			depthBufferInitInfo.queue = myQueue;
			depthBufferInitInfo.swapChain = mySwapChain;
			myDepthBuffer.initialize(depthBufferInitInfo);

			ADGRVulkanRenderPassInitInfo renderPassInitInfo;
			renderPassInitInfo.device = myDevice;
			renderPassInitInfo.global = myGlobals;
			renderPassInitInfo.enableDepthAttachment = true;
			renderPassInitInfo.swapChain = mySwapChain;
			myRenderPass.initialize(renderPassInitInfo);

			ADGRVulkanFrameBufferInitInfo frameBufferInitInfo;
			frameBufferInitInfo.device = myDevice;
			frameBufferInitInfo.renderPass = myRenderPass;
			frameBufferInitInfo.swapChain = mySwapChain;
			myFrameBuffer.initialize(frameBufferInitInfo);
		}
		
		void VulkanRBL3D::initStageTwo()
		{
		}
		
		void VulkanRBL3D::initStageThree()
		{
			mySyncObjects.initialize(myDevice);
		}
		
		void VulkanRBL3D::drawFrame()
		{
		}
		
		void VulkanRBL3D::shutDown()
		{
		}
		
		void VulkanRBL3D::shutDownStageOne()
		{
		}
		
		void VulkanRBL3D::shutDownStageTwo()
		{
		}
		
		void VulkanRBL3D::shutDownStageThree()
		{
		}
	}
}
/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "dmkafx.h"
#include "Renderer.h"
#include "defines.h"

#include "debugger.h"

#include "Platform/windows.h"

#include "Backend/Vulkan/vulkanRenderer.h"

namespace Dynamik {
	namespace ADGR {
		/* RBL declaration */
		static vulkanRenderer myVulkanRenderer;

		/* Renderer instance definition */
		Renderer Renderer::instance;
		
		void Renderer::initializeStageOne(DMKRenderingAPI selectedAPI, DMKRendererSettings settings)
		{
			switch (selectedAPI)
			{
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
				myVulkanRenderer = vulkanRenderer(settings);
				break;
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
				break;
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX_12:
				break;
			default:
				DMK_CORE_FATAL("Unsupported Rendering API!");
				break;
			}

			myVulkanRenderer.initStageOne();
		}

		void Renderer::initializeStageTwo()
		{
			myVulkanRenderer.initStageTwo();
		}

		void Renderer::initializeStageThree()
		{
			myVulkanRenderer.initStageThree();
		}

		void Renderer::setProgressPointer(POINTER<UI32> progress)
		{
			instance.progressPtr = progress;
			myVulkanRenderer.setProgress(progress.get());
		}

		void Renderer::setWindowHandle(POINTER<GLFWwindow> window)
		{
		}
		
		void Renderer::setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			instance.submitPendingAssets = formats;
		}
		
		void Renderer::submitLoadedAssets()
		{
		}

		void Renderer::addToRenderQueue(POINTER<InternalFormat> container)
		{
			instance.submitPendingAssets.pushBack(container);
		}

		void Renderer::drawFrame(DMKRendererDrawFrameInfo info)
		{
			myVulkanRenderer.drawFrame(info);
		}
		
		void Renderer::frameCleanup()
		{
		}

		void Renderer::terminate()
		{
			for (auto _container : instance.uniformBufferLocations)
				StaticAllocator<MAT4>::deAllocate(_container.location, _container.byteSize);
		}
	}
}
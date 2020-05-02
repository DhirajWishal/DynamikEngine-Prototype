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

#include "Backend/Vulkan/VulkanRBL.h"

namespace Dynamik {
	namespace ADGR {
		/* RBL declaration */
		VulkanRBL myRendererBackend;

		/* Renderer instance definition */
		Renderer Renderer::instance;

		void Renderer::setMSAASamples(DMKPipelineMSAASamples samples)
		{

		}

		UI32 Renderer::createRenderingInstance()
		{
			return UI32();
		}

		void Renderer::initializeRenderingInstance(UI32 instanceID)
		{
		}

		void Renderer::initializeStageOne(DMKRenderingAPI selectedAPI, DMKRendererSettings settings)
		{
			switch (selectedAPI)
			{
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
				break;
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
				break;
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX_12:
				break;
			default:
				DMK_CORE_FATAL("Unsupported Rendering API!");
				break;
			}

			myRendererBackend.initializeGraphicsCore();
		}

		void Renderer::initializeStageTwo()
		{
			myRendererBackend.initializeCommands();
		}

		void Renderer::initializeStageThree()
		{
			myRendererBackend.initializeFinalComponents();
		}

		void Renderer::setProgressPointer(POINTER<UI32> progress)
		{
			instance.progressPtr = progress;
			myRendererBackend.setProgress(progress);
		}

		void Renderer::setWindowHandle(POINTER<GLFWwindow> window)
		{
			myRendererBackend.setWindowHandle(window);
		}

		void Renderer::setWindowExtent(UI32 width, UI32 height)
		{
			myRendererBackend.setWindowExtent(width, height);
		}

		void Renderer::setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			DMK_BEGIN_PROFILE_TIMER();

			for (auto format : formats)
			{
				if (format->descriptor.assetDescription.physicallyBased)
				{
					format->descriptor.renderSpecification.renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_SKYBOX);
					format->descriptor.renderSpecification.renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_BRDF_TABLE);
					format->descriptor.renderSpecification.renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE);
					format->descriptor.renderSpecification.renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE);
				}

				instance.submitPendingAssets.pushBack(format);
			}
		}

		void Renderer::submitLoadedAssets()
		{
			DMK_BEGIN_PROFILE_TIMER();

			instance.inFlightAssets = instance.submitPendingAssets;
			instance.submitPendingAssets = {};
			myRendererBackend.addObjects(instance.inFlightAssets);
		}

		void Renderer::addToRenderQueue(POINTER<InternalFormat> container)
		{
			instance.submitPendingAssets.pushBack(container);
		}

		void Renderer::drawFrame(DMKRendererDrawFrameInfo info)
		{
			info.formats = instance.inFlightAssets;
			myRendererBackend.drawFrame(info);
		}

		void Renderer::frameCleanup()
		{
			DMK_BEGIN_PROFILE_TIMER();

		}

		void Renderer::terminate()
		{
			DMK_BEGIN_PROFILE_TIMER();

		}
	}
}
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
	namespace Renderer {
		/* RBL declaration */
		VulkanRBL myRendererBackend;

		/* Renderer instance definition */
		DMKRenderer DMKRenderer::instance;

		void DMKRenderer::setMSAASamples(DMKPipelineMSAASamples samples)
		{
			myRendererBackend.setMsaaSamples(samples);
		}

		UI32 DMKRenderer::createRenderingInstance()
		{
			return UI32();
		}

		void DMKRenderer::initializeRenderingInstance(UI32 instanceID)
		{
		}

		void DMKRenderer::initializeStageOne(DMKRenderingAPI selectedAPI, DMKRendererSettings settings)
		{
			switch (selectedAPI)
			{
			case Dynamik::Renderer::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
				break;
			case Dynamik::Renderer::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
				break;
			case Dynamik::Renderer::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX_12:
				break;
			default:
				DMK_CORE_FATAL("Unsupported Rendering API!");
				break;
			}

			myRendererBackend.initializeGraphicsCore();
		}

		UI32 DMKRenderer::createNewContext(DMKWindowHandle windowHandle, DMKRenderContextType contextType)
		{
			return UI32();
		}

		void DMKRenderer::initializeStageTwo()
		{
			myRendererBackend.initializeCommands();
		}

		void DMKRenderer::initializeStageThree()
		{
			myRendererBackend.initializeFinalComponents();
		}

		void DMKRenderer::setProgressPointer(POINTER<UI32> progress)
		{
			instance.progressPtr = progress;
			myRendererBackend.setProgress(progress);
		}

		void DMKRenderer::setWindowHandle(DMKWindowHandle windowHandle)
		{
			myRendererBackend.setWindowHandle(windowHandle);
		}

		void DMKRenderer::setWindowExtent(UI32 width, UI32 height)
		{
			myRendererBackend.setWindowExtent(width, height);
		}

		void DMKRenderer::setRenderableObjects(std::vector<POINTER<InternalFormat>> formats, UI32 contextID)
		{
			DMK_BEGIN_PROFILE_TIMER();

			for (auto format : formats)
			{
				if (format->descriptor.assetDescription.physicallyBased)
				{
					format->descriptor.renderSpecification.renderAttachments.push_back(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_SKYBOX);
					format->descriptor.renderSpecification.renderAttachments.push_back(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_BRDF_TABLE);
					format->descriptor.renderSpecification.renderAttachments.push_back(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE);
					format->descriptor.renderSpecification.renderAttachments.push_back(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE);
				}

				instance.submitPendingAssets.push_back(format);
			}
		}

		void DMKRenderer::submitLoadedAssets(UI32 contextID)
		{
			DMK_BEGIN_PROFILE_TIMER();

			instance.inFlightAssets = instance.submitPendingAssets;
			instance.submitPendingAssets = {};
			myRendererBackend.createNewContext(DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT, POINTER<GLFWwindow>());
			myRendererBackend.addObjects(instance.inFlightAssets);
		}

		void DMKRenderer::addToRenderQueue(POINTER<InternalFormat> container, UI32 contextID)
		{
			instance.submitPendingAssets.push_back(container);
		}

		void DMKRenderer::drawFrame(DMKRendererDrawFrameInfo info)
		{
			info.formats = instance.inFlightAssets;
			myRendererBackend.drawFrame(info);
		}

		void DMKRenderer::frameCleanup()
		{
			DMK_BEGIN_PROFILE_TIMER();

		}

		void DMKRenderer::terminate()
		{
			DMK_BEGIN_PROFILE_TIMER();

		}
	}
}
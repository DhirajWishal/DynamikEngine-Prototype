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

		/* Renderer instance definition */
		Renderer Renderer::instance;

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

			vulkanRenderer::initializeGraphicsCore();
		}

		void Renderer::initializeStageTwo()
		{
		}

		void Renderer::initializeStageThree()
		{
		}

		void Renderer::setProgressPointer(POINTER<UI32> progress)
		{
			instance.progressPtr = progress;
			vulkanRenderer::setProgress(progress);
		}

		void Renderer::setWindowHandle(POINTER<GLFWwindow> window)
		{
		}

		void Renderer::setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			for (auto format : formats)
			{

				if (format->type == DMKObjectType::DMK_OBJECT_TYPE_STATIC)
				{
					format->renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_SKYBOX);
					format->renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_BRDF_TABLE);
					format->renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE);
					format->renderAttachments.pushBack(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE);
				}

				instance.submitPendingAssets.pushBack(format);
			}
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
			vulkanRenderer::drawFrame(info);
		}

		void Renderer::frameCleanup()
		{
		}

		void Renderer::terminate()
		{
		}
	}
}
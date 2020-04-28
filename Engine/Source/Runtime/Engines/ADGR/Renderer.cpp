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

#include "Backend/DynamikRBL.h"

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
		}

		void Renderer::setWindowHandle(POINTER<GLFWwindow> window)
		{
		}

		void Renderer::setWindowExtent(UI32 width, UI32 height)
		{
		}

		void Renderer::setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats)
		{
			DMK_BEGIN_PROFILE_TIMER();

			for (auto format : formats)
			{
				for (auto attachment : format->descriptor.renderSpecification.renderAttachments)
				{
					switch (attachment)
					{
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_UNIFORM_BUFFER:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_TEXTURE:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_SKYBOX:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_LIGHTING:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_BRDF_TABLE:
						break;
					case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_COMPUTE_TEXTURE:
						break;
					}
				}

				instance.submitPendingAssets.pushBack(format);
			}
		}

		void Renderer::submitLoadedAssets()
		{
			DMK_BEGIN_PROFILE_TIMER();

			instance.inFlightAssets = instance.submitPendingAssets;
			instance.submitPendingAssets = {};
		}

		void Renderer::addToRenderQueue(POINTER<InternalFormat> container)
		{
			instance.submitPendingAssets.pushBack(container);
		}

		void Renderer::drawFrame(DMKRendererDrawFrameInfo info)
		{
			info.formats = instance.inFlightAssets;
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
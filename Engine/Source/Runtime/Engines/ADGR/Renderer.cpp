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

		void Renderer::initializeRenderContext(RenderContextType type)
		{
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
				switch (format->type)
				{
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
					initializeStaticObject(format);
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_CAMERA:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AUDIO:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_VIDEO:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_WIND:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SCRIPT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_KEY_BINDINGS:
					break;
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

		ADGR::StaticObject Renderer::initializeStaticObject(POINTER<InternalFormat> format)
		{
			ADGR::StaticObject _object;
			_object.myInternalFormat = format;

			_object.initializeResources();
		}
		
		ADGR::SkyBoxObject Renderer::initializeSkyBoxObject(POINTER<InternalFormat> format)
		{
			return ADGR::SkyBoxObject();
		}
		
		ADGR::DebugObject Renderer::initializeDebugObject(POINTER<InternalFormat> format)
		{
			return ADGR::DebugObject();
		}
	}
}
#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_RENDERER_RENDERER_H
#define _DYNAMIK_RENDERER_RENDERER_H


#include "rendererCommandQueue.h"

#include "GameObject.h"
#include "Objects/InternalFormat/InternalFormat.h"
#include "Managers/Asset/AssetManager.h"

#include "Camera.h"

namespace Dynamik {
	namespace Renderer {
		enum class DMKRenderingAPI {
			DMK_RENDERING_API_VULKAN,
			DMK_RENDERING_API_OPENGL,
			DMK_RENDERING_API_DIRECTX_12,
		};

		enum class DMKRenderContextType {
			DMK_RENDER_CONTEXT_TYPE_DEFAULT,
			DMK_RENDER_CONTEXT_TYPE_2D,
			DMK_RENDER_CONTEXT_TYPE_3D,
		};

		struct  DMKRendererSettings {
			std::string myWindowTitle = ("Dynamik Engine");
			UI32 myWindowWidth = 720;
			UI32 myWindowHeight = 480;
			ARRAY<std::string> myWindowIconPaths = { ("E:/Projects/Dynamik Engine/Dynamik/core assets/icons/Dynamik.jpg") };

			ARRAY<F32> clearScreenValues = {							// Vulkan clear screen values
				(2.0f / 256.0f),		// Red
				(8.0f / 256.0f),		// Green
				(32.0f / 256.0f),		// Blue
				(1.00000000f)			// Alpha
			};	// Dynwamik color code: rgba(2, 8, 32, 1)
		};

		struct  DMKRendererDrawFrameInfo {
			DMKCameraData cameraData;

			ARRAY<POINTER<InternalFormat>> formats;
		};

		/* RENDERER ABSTRACTION LAYER
		 * Singleton
		 */
		class DMKRenderer {
			DMKRenderer() {}

			static DMKRenderer instance;

			struct UniformBufferLocationContainer {
				POINTER<MAT4> location;
				UI32 byteSize = 0;
			};

		public:
			DMKRenderer(const DMKRenderer&) = delete;
			DMKRenderer(DMKRenderer&&) = delete;
			DMKRenderer& operator=(const DMKRenderer&) = delete;
			DMKRenderer& operator=(DMKRenderer&&) = delete;

			~DMKRenderer() {}

			/* Basic functions */
			static void setMSAASamples(DMKPipelineMSAASamples samples);

			static UI32 createRenderingInstance();
			static void initializeRenderingInstance(UI32 instanceID = 0);

			/* Initializing */
			static void initializeStageOne(DMKRenderingAPI selectedAPI, DMKRendererSettings settings);
			static UI32 createNewContext(POINTER<GLFWwindow> windowHandle, DMKRenderContextType contextType = DMKRenderContextType::DMK_RENDER_CONTEXT_TYPE_DEFAULT);
			static void initializeStageTwo();
			static void initializeStageThree();

			/* Setting containers and assets */
			static void setProgressPointer(POINTER<UI32> progress);
			static void setWindowHandle(POINTER<GLFWwindow> window);
			static void setWindowExtent(UI32 width, UI32 height);
			static void setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats, UI32 contextID = 0);
			static void submitLoadedAssets(UI32 contextID = 0);

			static void addToRenderQueue(POINTER<InternalFormat> format, UI32 contextID = 0);

			/* Draw call */
			static void drawFrame(DMKRendererDrawFrameInfo info);

			/* Termination */
			static void frameCleanup();
			static void terminate();
			
		private:
			POINTER<UI32> progressPtr;
			ARRAY<POINTER<InternalFormat>> inFlightAssets;
			ARRAY<POINTER<InternalFormat>> submitPendingAssets;
		};
	}
}

#endif // !_DYNAMIK_RENDERER_RENDERER_H
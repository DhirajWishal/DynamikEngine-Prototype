#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_RENDERER_H
#define _DYNAMIK_ADGR_RENDERER_H

#include "rendererFormat.h"
#include "core.h"
#include "rendererCommandQueue.h"

#include "GameObject.h"
#include "Objects/InternalFormat/InteralFormat.h"
#include "Managers/Asset/AssetManager.h"

#include "Camera.h"

namespace Dynamik {
	namespace ADGR {
		enum class DMK_API DMKRenderingAPI {
			DMK_RENDERING_API_VULKAN,
			DMK_RENDERING_API_OPENGL,
			DMK_RENDERING_API_DIRECTX_12,
		};

		struct DMK_API DMKRendererSettings {
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

		struct DMK_API DMKRendererDrawFrameInfo {
			DMKCameraData cameraData;
			F32 FOV = 60.0f;
			F32 aspectRatio = 1.0f;
			F32 frustumNear = 0.001f;
			F32 frustumFar = 256.0f;

			ARRAY<POINTER<InternalFormat>> formats;
		};

		/* RENDERER ABSTRACTION LAYER
		 * Singleton
		 */
		class DMK_API Renderer {
			Renderer() {}

			static Renderer instance;

			struct UniformBufferLocationContainer {
				POINTER<MAT4> location;
				UI32 byteSize = 0;
			};

		public:
			Renderer(const Renderer&) = delete;
			Renderer(Renderer&&) = delete;
			Renderer& operator=(const Renderer&) = delete;
			Renderer& operator=(Renderer&&) = delete;

			~Renderer() {}

			/* Initializing */
			static void initializeStageOne(DMKRenderingAPI selectedAPI, DMKRendererSettings settings);
			static void initializeStageTwo();
			static void initializeStageThree();

			/* Setting containers and assets */
			static void setProgressPointer(POINTER<UI32> progress);
			static void setWindowHandle(POINTER<GLFWwindow> window);
			static void setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats);
			static void submitLoadedAssets();

			static void addToRenderQueue(POINTER<InternalFormat> format);

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

#endif // !_DYNAMIK_ADGR_RENDERER_H
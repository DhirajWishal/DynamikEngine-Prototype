/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "Renderer.h"
#include "Renderer/backend/defines.h"

#ifdef DMK_PLATFORM_WINDOWS
#include "Platform/Windows.h"

#else

#endif

namespace Dynamik {
	namespace ADGR {

		Renderer::Renderer() {

		}

		Renderer::~Renderer() {

		}

		SMP_API void Renderer::initRenderer() {
			rendererCore.initWindow();
			rendererCore.startup();
		}

		void Renderer::draw() {
			rendererCore.drawFrame();
		}

		void Renderer::run() {
			while (!glfwWindowShouldClose(rendererCore.getWindow())) {
				glfwPollEvents();
				draw();
			}

			vkDeviceWaitIdle(rendererCore.getDevice());
		}

		void Renderer::end() {
			rendererCore.shutdown();
		}

		void Renderer::setMipLevel(float value) {
			rendererCore.setMipLevels(value);
		}

	}
}
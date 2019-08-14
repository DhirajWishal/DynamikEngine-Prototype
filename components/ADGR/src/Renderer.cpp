/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "Renderer.h"
#include "backend/defines.h"

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

		void Renderer::initRenderer() {
			rendererCore.initWindow();
			rendererCore.startup();
		}

		void Renderer::draw() {
			rendererCore.drawFrame();
		}

		void Renderer::run() {
			draw();
		}

		void Renderer::end() {
			rendererCore.shutdown();
		}

		void Renderer::setMipLevel(float value) {
			rendererCore.setMipLevels(value);
		}

		bool Renderer::getWindowCloseEvent() {
			return glfwWindowShouldClose(rendererCore.getWindow());
		}

		void Renderer::pollEvents() {
			glfwPollEvents();
		}

		void Renderer::idleCall() {
			vkDeviceWaitIdle(rendererCore.getDevice());
		}

	}
}

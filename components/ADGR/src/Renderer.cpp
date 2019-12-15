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

#include "backend/vulkan/ADGR_Vulkan_API.h"
#include "debugger.h"

#ifdef DMK_PLATFORM_WINDOWS
#include "Platform/Windows.h"

#else

#endif

namespace Dynamik {
	namespace ADGR {
		vulkanRenderer rendererCore;

		Renderer::Renderer() {
		}

		Renderer::~Renderer() {
		}

		void Renderer::setProgress(uint32_t* progress) {
			rendererCore.setProgress(progress);
		}

		void Renderer::initRenderer() {
			rendererCore.init();
		}

		void Renderer::setRendererFormats(std::vector<InternalFormat*>& internalFormats) {
			for (int i = 0; i < internalFormats.size(); i++)
				myRendererFormats.push_back(RendererFormat(internalFormats[i]));

			rendererCore.setRendererFormats(myRendererFormats);
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

		bool Renderer::getWindowCloseEvent() {
			return rendererCore.closeEvent();
		}

		void Renderer::idleCall() {
			vkDeviceWaitIdle(rendererCore.getDevice());
		}

		void Renderer::bindKeys() {
		}

		std::deque<DMKEventContainer> Renderer::getEvents() {
			return rendererCore.events();
		}
	}
}
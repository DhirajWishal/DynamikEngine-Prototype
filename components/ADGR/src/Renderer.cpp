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

		void Renderer::initRenderer() {
			rendererCore.init();
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
		}

		bool Renderer::getWindowCloseEvent() {
			return rendererCore.closeEvent();
		}

		std::tuple<keyEventData*, mouseEventData*> Renderer::pollEvents() {
			rendererCore.events();

			keyEventData* prop = nullptr;

			auto [ked, med] = rendererCore.getEvent();
			
			if (ked != prop)
				return rendererCore.getEvent();
			return { nullptr, nullptr };
		}

		void Renderer::idleCall() {
			vkDeviceWaitIdle(rendererCore.getDevice());
		}

		void Renderer::bindKeys() {

		}

		void Renderer::setAssetPaths(std::vector<std::string>& texture, std::vector<std::string>& model) {
			//rendererCore.setAssets(texture, model);
		}

	}
}

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

		void Renderer::initRenderer(initInfo::ADGR info) {
			rendererCore.setModelPaths(info.modelPaths, info.texturePaths);
			rendererCore.setProgress(info.progress);

			rendererCore.init();
		}

		void Renderer::getGameObjects(std::vector<GameObject>& gameObjects) {
			rendererCore.setGameObjects(gameObjects);
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

		std::tuple<int, mouseEventData*> Renderer::pollEvents() {
			rendererCore.events();

			int prop = NULL;

			auto [ked, med] = rendererCore.getEvent();

			if (ked != prop)
				return rendererCore.getEvent();
			return { NULL, nullptr };
		}

		void Renderer::idleCall() {
			vkDeviceWaitIdle(rendererCore.getDevice());
		}

		void Renderer::bindKeys() {
		}
	}
}
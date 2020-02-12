#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_window_H
#define _DYNAMIK_ADGR_VULKAN_window_H
#ifdef DMK_USE_VULKAN

#include <deque>

#include "keyEvent.h"
#include "mouseEvent.h"
#include "backend/vulkan/core/data structures/vulkan.h"
#include "CentralDataHub.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class windowManager {
			public:
				windowManager() {}
				~windowManager() {}

				void init(ADGRVulkanDataContainer* container);
				void clear(ADGRVulkanDataContainer* container);

				B1 createWindowSurface(ADGRVulkanDataContainer* container);

				void pollEvents();
				void onUpdate();
				B1 closeEvent(ADGRVulkanDataContainer* container);

				void onWindowResizeEvent(ADGRVulkanDataContainer* container);

				void setWindowIcon(ADGRVulkanDataContainer* container, std::vector<std::string> paths);

				B1 isFrameBufferResized() { return frameBufferResized; }
				void frameBufferResizedUpdate(B1 state) { frameBufferResized = state; }

				std::deque<DMKEventContainer> getEventContainer() {
					auto container = eventContainer;
					eventContainer.clear();
					return container;
				}

			private:
				std::deque<DMKEventContainer> eventContainer = {};
				void keyEventHandler(DMKEventType type, I32 keycode = -1, I32 count = 0);
				void mouseButtonEvent(DMKEventType type, I32 keycode = -1, I32 count = 0);
				void mouseScrolledEvent(F32 xOffset = 0.0f, F32 yOffset = 0.0f);
				void mouseMovedEvent(F32 xOffset = 0.0f, F32 yOffset = 0.0f);

				void _drawOnce();

				using eventCallbackFunction = std::function<void(Event&, I32)>;

				const UI32 HEIGHT = 720;
				const UI32 WIDTH = 1280;

				B1 frameBufferResized = false;

				static void framebufferResizeCallback(GLFWwindow* win, I32 width, I32 height);
				static void onKeyEvent(GLFWwindow* window, I32 keycode, I32 scancode,
					I32 action, I32 mods);
				static void onMouseButtonEvent(GLFWwindow* window, I32 button, I32 action, I32 mods);
				static void onMouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);
				static void onCursorPosEvent(GLFWwindow* window, double xPos, double yPos);
				static void onwindowCloseEvent(GLFWwindow* window);

				void setEventCallback(const eventCallbackFunction& callback) {
					myEventCallbackFunction = callback;
				}

				eventCallbackFunction myEventCallbackFunction;
			};
		}
	}
}

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_window_H

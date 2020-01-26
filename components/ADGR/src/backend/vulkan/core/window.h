#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_window_H
#define _DYNAMIK_ADGR_VULKAN_window_H

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

				bool createwindowSurface(ADGRVulkanDataContainer* container);

				void pollEvents();
				void onUpdate();
				bool closeEvent(ADGRVulkanDataContainer* container);

				void onwindowResizeEvent(ADGRVulkanDataContainer* container);

				void setwindowIcon(ADGRVulkanDataContainer* container, std::vector<std::string> paths);

				bool isFrameBufferResized() { return frameBufferResized; }
				void frameBufferResizedUpdate(bool state) { frameBufferResized = state; }

				std::deque<DMKEventContainer> getEventContainer() {
					auto container = eventContainer;
					eventContainer.clear();
					return container;
				}

			private:
				std::deque<DMKEventContainer> eventContainer = {};
				void keyEventHandler(DMKEventType type, int keycode = -1, int count = 0);
				void mouseButtonEvent(DMKEventType type, int keycode = -1, int count = 0);
				void mouseScrolledEvent(float xOffset = 0.0f, float yOffset = 0.0f);
				void mouseMovedEvent(float xOffset = 0.0f, float yOffset = 0.0f);

				void _drawOnce();

				using eventCallbackFunction = std::function<void(Event&, int)>;

				const unsigned int HEIGHT = 720;
				const unsigned int WIDTH = 1280;

				bool frameBufferResized = false;

				static void framebufferResizeCallback(GLFWwindow* win, int width, int height);
				static void onKeyEvent(GLFWwindow* window, int keycode, int scancode,
					int action, int mods);
				static void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
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

#endif // !_DYNAMIK_ADGR_VULKAN_window_H

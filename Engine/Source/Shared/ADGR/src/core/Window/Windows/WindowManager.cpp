#include "adgrafx.h"
#include "WindowManager.h"

namespace Dynamik {
	namespace ADGR {
		void WindowManager::initialize(DMKWindowManagerInitInfo info)
		{
			windowWidth = info.width;
			windowHeight = info.height;

			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#if defined(DMK_DEBUG)
			window = glfwCreateWindow(windowWidth, windowHeight, info.title.c_str(), nullptr, nullptr);

#else
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			window = glfwCreateWindow(mode->width, mode->height, info.title.c_str(), monitor, NULL);
			windowWidth = mode->width;
			windowHeight = mode->height;

#endif
			glfwMakeContextCurrent(window);

			glfwSetWindowUserPointer(window, this);
			glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
			glfwSetKeyCallback(window, onKeyEvent);
			glfwSetMouseButtonCallback(window, onMouseButtonEvent);
			glfwSetScrollCallback(window, onMouseScrolledEvent);
			glfwSetCursorPosCallback(window, onCursorPosEvent);
			glfwSetWindowCloseCallback(window, onwindowCloseEvent);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			setIcon(info.iconPaths);
		}

		void WindowManager::terminate()
		{
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		void WindowManager::pollEvents() {
			glfwPollEvents();
		}

		void WindowManager::onUpdate() {
			pollEvents();
		}

		B1 WindowManager::createWindowSurface(POINTER<Backend::VulkanInstance> instance)
		{
			if (glfwCreateWindowSurface(instance->instance, window, nullptr, &instance->surface) != VK_SUCCESS) {
				DMK_CORE_FATAL("Failed to create window surface!");
				return false;
			}

			return true;
		}

		void WindowManager::setIcon(std::string paths)
		{
			ARRAY<GLFWimage> icons;
			resource::TextureData texDatas;

			for (UI32 i = 0; i < 1; i++) {
				std::string_view path = paths;
				GLFWimage icon;
				auto pixels = texDatas.loadTexture((path.data()), resource::TEXTURE_TYPE_RGBA);

				if (!pixels)
					DMK_CORE_ERROR("Icon not Loaded!");

				icon.pixels = pixels;
				icon.height = texDatas.texHeight;
				icon.width = texDatas.texWidth;
				icons.push_back(icon);
			}

			glfwSetWindowIcon(window, 1, icons.data());
		}

		void WindowManager::keyEventHandler(DMKEventType type, I32 keycode, I32 count) {
			DMKEventContainer _localContainer = {};
			_localContainer.eventType = type;
			_localContainer.code = keycode;
			_localContainer.count = count;
			eventContainer.push_back(_localContainer);
		}

		void WindowManager::mouseButtonEvent(DMKEventType type, I32 keycode, I32 count) {
			DMKEventContainer _localContainer = {};
			_localContainer.eventType = type;
			_localContainer.code = keycode;
			_localContainer.count = count;
			eventContainer.push_back(_localContainer);
		}

		void WindowManager::mouseScrolledEvent(F32 xOffset, F32 yOffset) {
			DMKEventContainer _localContainer = {};
			_localContainer.eventType = DMKEventType::DMK_EVENT_TYPE_MOUSE_SCROLL;
			_localContainer.xOffset = xOffset;
			_localContainer.yOffset = yOffset;
			eventContainer.push_back(_localContainer);
		}

		void WindowManager::mouseMovedEvent(F32 xOffset, F32 yOffset) {
			DMKEventContainer _localContainer = {};
			_localContainer.eventType = DMKEventType::DMK_EVENT_TYPE_MOUSE_MOVED;
			_localContainer.xAxis = xOffset;
			_localContainer.yAxis = yOffset;
			eventContainer.push_back(_localContainer);
		}

		void WindowManager::framebufferResizeCallback(GLFWwindow* win, I32 width, I32 height) {
			auto app = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(win));
			app->frameBufferResized = true;
		}

		void WindowManager::onKeyEvent(GLFWwindow* win, I32 keycode, I32 scancode, I32 action, I32 mods) {
			auto data = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(win));
			data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT, -1);

			switch (action) {
			case DMK_PRESS: {
				data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_PRESS, keycode);

				if (keycode == DMK_KEY_ESCAPE)
					glfwSetWindowShouldClose(win, GL_TRUE);
				break;
			}
			case DMK_BUTTON_RELEASE: {
				data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_RELEASE, keycode);
				break;
			}
			case DMK_REPEAT: {
				data->keyEventHandler(DMKEventType::DMK_EVENT_TYPE_KEY_REPEAT, keycode, 1);
				break;
			}
			}
		}

		void WindowManager::onMouseButtonEvent(GLFWwindow* win, I32 button, I32 action, I32 mods) {
			auto data = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(win));
			data->mouseButtonEvent(DMKEventType::DMK_EVENT_TYPE_UNKNOWN, -1);

			switch (action) {
			case DMK_PRESS: {
				data->mouseButtonEvent(DMKEventType::DMK_EVENT_TYPE_MOUSE_PRESS, button);
				break;
			}
			case DMK_BUTTON_RELEASE: {
				data->mouseButtonEvent(DMKEventType::DMK_EVENT_TYPE_MOUSE_RELEASE, button);
				break;
			}
			}
		}

		void WindowManager::onMouseScrolledEvent(GLFWwindow* win, double xOffset, double yOffset) {
			auto data = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(win));
			data->mouseScrolledEvent(xOffset, yOffset);
		}

		void WindowManager::onCursorPosEvent(GLFWwindow* win, double xPos, double yPos) {
			auto data = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(win));
			data->mouseMovedEvent(xPos, yPos);
		}

		void WindowManager::onwindowCloseEvent(GLFWwindow* window) {
		}
	}
}
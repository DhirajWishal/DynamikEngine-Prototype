#include "dmkafx.h"
#include "WindowsWindow.h"

#include <GLEW/glew-2.1.0/include/GL/glew.h>

#include "Objects/Texture/Texture.h"

#include "Events/EventManager.h"

namespace Dynamik {
	void WindowsWindow::initialize(DMKWindowInitInfo info)
	{
		DMK_BEGIN_PROFILE_TIMER();

		windowWidth = info.width;
		windowHeight = info.height;

		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

#if defined(DMK_DEBUG) || defined(DMK_RELEASE)
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

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

#endif

		DMKEventManager::setEventCallbacks(window);

		glfwSetWindowUserPointer(window, this);

		//setIcon(info.iconPaths);
	}

	void WindowsWindow::terminate()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowsWindow::pollEvents() {
		glfwPollEvents();
	}

	void WindowsWindow::onUpdate() {
		pollEvents();
	}

	void WindowsWindow::setIcon(std::string paths)
	{
		std::vector<GLFWimage> icons;

		for (UI32 i = 0; i < 1; i++) {
			std::string_view path = paths;
			GLFWimage icon;
			Texture myTexture;
			myTexture.loadTexture(path.data(), DMKTextureType::DMK_TEXTURE_TYPE_2D, DMKTextureInputType::DMK_TEXTURE_INPUT_TYPE_IMAGE);

			if (!myTexture.textureData.isValid())
				DMK_CORE_ERROR("Icon not Loaded!");

			icon.pixels = myTexture.textureData;
			icon.height = myTexture.height;
			icon.width = myTexture.width;
			icons.push_back(icon);
		}

		glfwSetWindowIcon(window, 1, icons.data());
	}

	void WindowsWindow::updateWindowSize()
	{
		I32 width = 0, height = 0;
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		windowWidth = width;
		windowHeight = height;
	}
}
#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "event.h"

#include <GLFW/glfw3.h>

namespace Dynamik {
	namespace ADGR {
		namespace windows {

			struct windowProperties {
				std::string title = {};
				unsigned int width = 0;
				unsigned int height = 0;

				bool vSync = false;

				windowProperties(
					const std::string& title = "Dynamik Engine",
					unsigned int width = 1280,
					unsigned int height = 720
				) : title(title), width(width), height(height) {
				}
			};

			class Window {
			public:
				using eventCallbackFunction = std::function<void(Event&)>;

				Window(windowProperties props) :
					myProps(props) {}
				Window() {}
				~Window() {}

				void onUpdate();

				unsigned int getWidth() { return myProps.width; }
				unsigned int getHeight() { return myProps.height; }

				void setEventCallback(const eventCallbackFunction& callback)
				{
					eventData.myEventCallbackFunction = callback;
				}

				void setVSync(bool enabled) { myProps.vSync = enabled; }
				bool isVSync() { return myProps.vSync; }

				void create();
				void setFramebufferResizeCallback() {}

				GLFWwindow* getWindow() { return myWindow; }

			private:
				GLFWwindow* myWindow = nullptr;

				windowProperties myProps = {};

				struct events {
					windowProperties myProps = myProps;

					eventCallbackFunction myEventCallbackFunction;
				}eventData;
			};
		}
	}
}

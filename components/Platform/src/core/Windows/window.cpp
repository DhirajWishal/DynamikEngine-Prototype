/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "window.h"

#include "keyEvent.h"
#include "mouseEvent.h"
#include "applicationEvent.h"

#include <stdio.h>

namespace Dynamik {
	namespace ADGR {
		namespace windows {

			static bool isGLFWInitialized = false;

			static void GLFWErrorCallback(int error, const char* description) {
				printf("Error: %d -> %s\n", error, description);
			}

			void Window::create() {
				if (!isGLFWInitialized) {
					// TODO: GLFW terminate on shutdown
					int success = glfwInit();

					//DMK_CORE_ASSERT(success, "Unable to initialize window")
					glfwSetErrorCallback(GLFWErrorCallback);
					isGLFWInitialized = true;
				}

				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

				myWindow = glfwCreateWindow(myProps.width, myProps.height,
					myProps.title.c_str(), nullptr, nullptr);
				glfwMakeContextCurrent(myWindow);
				//glfwSetWindowUserPointer(myWindow, &myProps);
				setVSync(true);

				//glfwSetKeyCallback(myWindow, [](GLFWwindow* window, int keycode, int scancode, int action, int mods) {
				//	events& data = *(events*)glfwGetWindowUserPointer(window);
				//
				//	switch (action) {
				//		case GLFW_PRESS: {
				//			KeyPressedEvent event(keycode, 0);
				//			data.myEventCallbackFunction(event);
				//
				//			break;
				//		}
				//		case GLFW_RELEASE: {
				//			KeyReleasedEvent event(keycode);
				//			data.myEventCallbackFunction(event);
				//
				//			break;
				//		}
				//		case GLFW_REPEAT: {
				//			KeyPressedEvent event(keycode, 1);
				//			data.myEventCallbackFunction(event);
				//
				//			break;
				//		}
				//	}
				//});
				//
				//glfwSetMouseButtonCallback(myWindow, [](GLFWwindow* window, int button, int action, int mods) {
				//	events& data = *(events*)glfwGetWindowUserPointer(window);
				//
				//	switch (action)
				//	{
				//		case GLFW_PRESS: {
				//			MouseButtonPressedEvent event(button);
				//			data.myEventCallbackFunction(event);
				//
				//			break;
				//		}
				//		case GLFW_RELEASE: {
				//			MouseButtonReleasedEvent event(button);
				//			data.myEventCallbackFunction(event);
				//
				//			break;
				//		}
				//		default:
				//			break;
				//	}
				//});
				//
				//glfwSetScrollCallback(myWindow, [](GLFWwindow* window, double xOffset, double yOffset) {
				//	events& data = *(events*)glfwGetWindowUserPointer(window);
				//	
				//	MouseScrolledEvent event((float)xOffset, (float)yOffset);
				//	data.myEventCallbackFunction(event);
				//});
				//
				//glfwSetCursorPosCallback(myWindow, [](GLFWwindow* window, double xPos, double yPos) {
				//	events& data = *(events*)glfwGetWindowUserPointer(window);
				//
				//	MouseMovedEvent event((float)xPos, (float)yPos);
				//	data.myEventCallbackFunction(event);
				//});
			}

			void Window::onUpdate() {
				glfwPollEvents();
			}
		}
	}
}

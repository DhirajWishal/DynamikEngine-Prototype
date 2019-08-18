/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "window.h"



namespace Dynamik {
	namespace ADGR {
		namespace windows {

			static bool isGLFWInitialized = false;

			Window* Window::create(const windowProperties& props) {
				return new windowsWindow(props);
			}

			windowsWindow::windowsWindow(const windowProperties& props) {
				init(props);
			}

			windowsWindow::~windowsWindow() {
				shutdown();
			}

			void windowsWindow::init(const windowProperties& props) {
				myData.myTitle = props.title;
				myData.myWidth = props.width;
				myData.myHeight = props.height;

				//DMK_CORE_INFO("Created window!");

				if (!isGLFWInitialized) {
					// TODO: GLFW terminate on shutdown
					int success = glfwInit();

					//DMK_CORE_ASSERT(success, "Unable to initialize window")

					isGLFWInitialized = true;
				}

				myWindow = glfwCreateWindow(myData.myWidth, myData.myHeight,
					myData.myTitle.c_str(), nullptr, nullptr);
				glfwMakeContextCurrent(myWindow);
				glfwSetWindowUserPointer(myWindow, &myData);
				setVSync(true);
			}

			void windowsWindow::shutdown() {
				glfwDestroyWindow(myWindow);
			}

			void windowsWindow::onUpdate() {
				glfwPollEvents();
				// swap buffers on vulkan
			}

			void windowsWindow::setVSync(bool state) {
				if (state)
					;
				else
					;
				myData.myVSync = state;
			}
			bool windowsWindow::isVSync() const
			{
				return false;
			}
		}
	}
}

#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "event.h"

namespace Dynamik {
	namespace ADGR {
		namespace windows {

			struct windowProperties {
				std::string title;
				unsigned int width;
				unsigned int height;

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

				virtual ~Window() {}

				virtual void onUpdate() = 0;

				virtual unsigned int getWidth() const = 0;
				virtual unsigned int getHeight() const = 0;

				virtual void setEventCallback(const eventCallbackFunction& callback) = 0;
				virtual void setVSync(bool enabled) = 0;
				virtual bool isVSync() const = 0;
				static Window* create(const windowProperties& props = windowProperties());

				virtual GLFWwindow* getWindow() { return nullptr; }
			};

			// derived class
			class windowsWindow : public Window {
			public:
				windowsWindow(const windowProperties& prop);
				virtual ~windowsWindow();

				void onUpdate() override;

				inline unsigned int getWidth() const override { return myData.myWidth; }
				inline unsigned int getHeight() const override { return myData.myHeight; }

				inline void setEventCallback(const eventCallbackFunction& callback) override {
					myData.myEventCallback = callback;
				}
				void setVSync(bool enabled) override;
				bool isVSync() const override;

				GLFWwindow* getWindow() override { return myWindow; }

			protected:
				virtual void init(const windowProperties& props);
				virtual void shutdown();

			private:
				GLFWwindow* myWindow = nullptr;

				struct windowData {
					std::string myTitle;
					unsigned int myWidth, myHeight;
					bool myVSync;

					eventCallbackFunction myEventCallback;
				} myData;
			};
		}
	}
}

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "smpafx.h"
#include "Renderer.h"

namespace Dynamik {
	namespace ADGR {

		Renderer::Renderer() {
			myWindow.init();

			myInstance.init();
			myDebugger.setupDebugMessenger();

			myWindow.createWindowSurface();

			mySwapChain.init();
			mySwapChain.initImageViews();

			myPipeline.initRenderPass();

			myDevice.init();
		}

		Renderer::~Renderer() {


			// final
			myWindow.clear();
		}
	}
}

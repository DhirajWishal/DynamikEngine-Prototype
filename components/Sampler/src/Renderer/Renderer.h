#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "backend/backend.h"

namespace Dynamik {
	namespace ADGR {
		using namespace core;
		
		class Renderer {
		public:
			Renderer();
			~Renderer();

		private:
			window myWindow;

			instance myInstance;
			device myDevice;
			swapChain mySwapChain;
			pipeline myPipeline;

			debugger myDebugger{myInstance.getInstanceAddr(), myInstance.getdebugMessengerAddr()};
		};
	}
}

#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "backend/interface.h"
#include "backend/core.h"

#include "Platform.h"

namespace Dynamik {
	namespace ADGR {

		class ADGR_API Renderer {
		public:
			Renderer();
			~Renderer();

			void initRenderer();
			void draw();

			void run();

			void end();

			// core functions

			bool getWindowCloseEvent();
			void pollEvents();
			void idleCall();

			void setMipLevel(float);

		private:
			core::core rendererCore;
		};
	}
}

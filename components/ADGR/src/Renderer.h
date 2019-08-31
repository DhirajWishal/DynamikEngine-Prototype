#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "backend/interface.h"
#include "core/core.h"

#include "Platform.h"

#include "keyEvent.h"
#include "mouseEvent.h"

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

			void bindKeys();

			// core functions

			bool getWindowCloseEvent();
			std::tuple<keyEventData*, mouseEventData*> pollEvents();
			void idleCall();

			void setMipLevel(float);

			void setAssetPaths(std::vector<std::string>& texture, std::vector<std::string>& model);
		};
	}
}

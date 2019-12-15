#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_RENDERER_H
#define _DYNAMIK_ADGR_RENDERER_H

#include "backend/interface.h"
#include "backend/vulkan/core/vulkanFormat.h"
#include "core/core.h"

#include "Platform.h"

#include "GameObject.h"
#include "keyEvent.h"
#include "mouseEvent.h"
#include <deque>

#include "core/utils/DMK_Descriptors.h"

namespace Dynamik {
	namespace ADGR {
		class ADGR_API Renderer {
		public:
			Renderer();
			~Renderer();

			void setProgress(uint32_t* progress);
			void initRenderer();
			void setRendererFormats(std::vector<InternalFormat*>& internalFormats);
			void draw();

			void run();

			void end();

			void bindKeys();
			std::deque<DMKEventContainer> getEvents();

			// core functions

			bool getWindowCloseEvent();
			void idleCall();

			std::vector<RendererFormat> myRendererFormats = {};
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_H

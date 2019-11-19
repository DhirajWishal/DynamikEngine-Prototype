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
#include "core/core.h"

#include "Platform.h"

#include "GameObject/GameObject.h"
#include "keyEvent.h"
#include "mouseEvent.h"

#include "core/utils/DMK_Descriptors.h"

namespace Dynamik {
	namespace ADGR {
		class ADGR_API Renderer {
		public:
			Renderer();
			~Renderer();

			void initRenderer(initInfo::ADGR info);
			void getGameObjects(std::vector<GameObject>& gameObjects);
			void draw();

			void run();

			void end();

			void bindKeys();

			// core functions

			bool getWindowCloseEvent();
			std::tuple<int, mouseEventData*> pollEvents();
			void idleCall();
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_H

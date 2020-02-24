#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_RENDERER_H
#define _DYNAMIK_ADGR_RENDERER_H

#include "interface.h"
#include "rendererFormat.h"
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

			void setProgress(UI32* progress);
			void initRenderer();
			void setRendererFormats(std::vector<InternalFormat*>& internalFormats);
			void draw();

			void setVertices(std::vector<Vertex>* vertices);

			void run();
			void loadDataToUpdate(std::vector<InternalFormat*>& internalFormats);
			void updateRendererFormats();
			void loadData(std::vector<InternalFormat*>& internalFormats, std::vector<RendererFormat>* formats);

			void end();

			void bindKeys();
			std::deque<DMKEventContainer>* getEvents();

			// core functions

			B1 getWindowCloseEvent();
			void idleCall();

			std::vector<RendererFormat> myRendererFormats = {};
			std::vector<RendererFormat> myTemporaryFormats;

			std::vector<Vertex> localContainer;
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_H

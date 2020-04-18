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
#include "rendererCommandQueue.h"

#include "Platform.h"

#include "GameObject.h"
#include "keyEvent.h"
#include "mouseEvent.h"
#include <deque>

#include "core/utils/DMK_Descriptors.h"
#include "core/AssetIndexBlock.h"

namespace Dynamik {
	namespace ADGR {
		/* RENDERER ABSTRACTION LAYER
		 *
		 */
		class ADGR_API Renderer {
		public:
			Renderer();
			~Renderer();

			void setProgress(UI32* progress);
			void initRenderer();
			void setRendererFormats(ARRAY<DMKAssetIndexContainer>& containers);
			void draw();

			void setVertices(ARRAY<Vertex>* vertices);

			void addCommand(RendererCommandQueue commandQueue);

			void run();
			void loadDataToUpdate(ARRAY<DMKAssetIndexContainer>& containers);
			void updateRendererFormats();
			void loadData(ARRAY<DMKAssetIndexContainer>& containers, ARRAY<RendererFormat>* formats);

			void end();

			void bindKeys();
			std::deque<DMKEventContainer>* getEvents();

			// core functions

			B1 getWindowCloseEvent();
			void idleCall();

			ARRAY<RendererFormat> myRendererFormats = {};
			ARRAY<RendererFormat> myTemporaryFormats;

			ARRAY<Vertex> localContainer;
			RendererCommandQueue myCommandQueue;
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_H

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

#include "rendererFormat.h"
#include "core/core.h"
#include "rendererCommandQueue.h"

#include "Platform.h"

#include "GameObject.h"
#include "core/Interface/Events/DMKEventComponent.h"

#include "core/utils/DMK_Descriptors.h"

namespace Dynamik {
	namespace ADGR {
		/* RENDERER ABSTRACTION LAYER
		 *
		 */
		class DMK_API Renderer {
		public:
			Renderer();
			~Renderer();

			void setProgress(UI32* progress);
			void initRenderer();
			void setRendererFormats(ARRAY<InternalFormat*>& internalFormats);
			void draw(ARRAY<DMKEventComponent*> events);

			void setVertices(ARRAY<Vertex>* vertices);

			void addCommand(RendererCommandQueue commandQueue);

			void loadDataToUpdate(ARRAY<InternalFormat*>& internalFormats);
			void updateRendererFormats();
			void loadData(ARRAY<InternalFormat*>& internalFormats, ARRAY<RendererFormat>* formats);

			void end();

			void bindKeys();

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
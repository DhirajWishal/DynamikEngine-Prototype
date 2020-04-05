#pragma once

#ifndef _DYNAMIK_ADGR_RENDERER_BACKEND_BASE_H
#define _DYNAMIK_ADGR_RENDERER_BACKEND_BASE_H

/*
 Renderer Backend Base class

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		26/01/2020
 IDE:		MS Visual Studio Community 2019

 RendererBackendBase.h file

 The Renderer Backend is the library which contains the objects and functions which does the specified calls
 with the help of different rendering APIs. Renderer class will do the rendering by utilizing the backend
 class(es).
 Supported API:
 * Vulkan (main)
 * DirectX 12
 * OpenGL
*/

#include <cstdint>
#include <vector>
#include <string>
#include <deque>

#include <CentralDataHub.h>
#include <rendererFormat.h>

#define INC_PROGRESS					(*myProgress += 1)
#define MULTIPLE_INC_PROGRESS(count)	for (I32 i_itr__ = 0; i_itr__ < count; i_itr__++) INC_PROGRESS

namespace Dynamik {
	namespace ADGR {
		struct DMKRendererSettings {
			std::string myWindowTitle = ("Dynamik Engine");
			UI32 myWindowWidth = 720;
			UI32 myWindowHeight = 480;
			ARRAY<std::string> myWindowIconPaths = { ("E:/Projects/Dynamik Engine/Dynamik/core assets/icons/Dynamik.jpg") };

			ARRAY<F32> clearScreenValues = {							// Vulkan clear screen values
				(2.0f / 256.0f),		// Red
				(8.0f / 256.0f),		// Green
				(32.0f / 256.0f),		// Blue
				(1.00000000f)			// Alpha
			};	// Dynwamik color code: rgba(2, 8, 32, 1)
		};

		class RendererBackend {
		public:
			RendererBackend() {}	// constructor
			RendererBackend(DMKRendererSettings settings) : myRendererSettings(settings) {}	// constructor
			virtual ~RendererBackend() {}	// destructor

			/* VIRTUAL FUNCTION(S) */
			virtual void init() {}	// initialize the renderer(full)
			virtual void initStageOne() {}	// initialize stage one
			virtual void initStageTwo() {}	// initialize stage two
			virtual void initStageThree() {}	// initialize stage three

			virtual void drawFrame(std::deque<DMKEventContainer> container) {}	// draw call

			virtual void shutDown() {}	// shut down the renderer
			virtual void shutDownStageOne() {}	// shut down stage one
			virtual void shutDownStageTwo() {}	// shut down stage two
			virtual void shutDownStageThree() {}	// shut down stage three

			virtual void setFormats3D(ARRAY<RendererFormat>& formats) {}	// set formats to the renderer
			virtual void updateFormats3D(ARRAY<RendererFormat>& formats) {}	// update formats in the renderer
			virtual void setFormats2D(ARRAY<RendererFormat>& formats) {}	// set formats to the renderer
			virtual void updateFormats2D(ARRAY<RendererFormat>& formats) {}	// update formats in the renderer

			void setShaderPaths(ARRAY<std::string> paths) { myShaderPaths = paths; }	// set shader paths
			void setObjectAndTexturePaths(ARRAY<std::string>& object, ARRAY<ARRAY<std::string>>& texture) {	// set object and texture paths
				myModelPaths = object;
				myTexturePaths = texture;
			}

			virtual std::deque<DMKEventContainer>* events() { return &std::deque<DMKEventContainer>(); }	// get event containers
			virtual B1 closeEvent() { return false; }	// get close event

			/* DEFINED FUNCTION(S) */
			void setProgress(UI32* progress) { myProgress = progress; }	// set progress
			void setRendererSettings(DMKRendererSettings settings) { myRendererSettings = settings; }	// set renderer settings

		protected:
			DMKRendererSettings myRendererSettings = {};	// renderer settings

			std::deque<DMKEventContainer> myEventContainers = {};	// event containers

			ARRAY<std::string> myModelPaths = {};	// model paths
			ARRAY<ARRAY<std::string>> myTexturePaths = {};	// texture paths
			ARRAY<std::string> myShaderPaths = {};	// shader paths

			UI32* myProgress = 0;	// progress counter
			B1 compileShaders = false;	// compile shaders command
			UI32 myFormatsCount = 0;	// format counter
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_BASE_H

#pragma once

#ifndef _DYNAMIK_ADGR_RENDERER_BACKEND_BASE_H
#define _DYNAMIK_ADGR_RENDERER_BACKEND_BASE_H

/*
 Renderer Backend Base class

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
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
#define MULTIPLE_INC_PROGRESS(count)	for (int i_itr__ = 0; i_itr__ < count; i_itr__++) INC_PROGRESS

namespace Dynamik {
	namespace ADGR {
		struct DMKRendererSettings {
			std::string myWindowTitle = "Dynamik Engine";
			uint32_t myWindowWidth = 720;
			uint32_t myWindowHeight = 480;
			std::vector<std::string> myWindowIconPaths = { "E:/Projects/Dynamik Engine/Dynamik/core assets/icons/Dynamik.jpg" };

			std::vector<float> clearScreenValues = {							// Vulkan clear screen values
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

			virtual void drawFrame() {}	// draw call

			virtual void shutDown() {}	// shut down the renderer
			virtual void shutDownStageOne() {}	// shut down stage one
			virtual void shutDownStageTwo() {}	// shut down stage two
			virtual void shutDownStageThree() {}	// shut down stage three

			virtual void setFormats(std::vector<RendererFormat>& formats) {}	// set formats to the renderer
			virtual void updateFormats(std::vector<RendererFormat>& formats) {}	// update formats in the renderer

			void setShaderPaths(std::vector<std::string> paths) { myShaderPaths = paths; }	// set shader paths
			void setObjectAndTexturePaths(std::vector<std::string>& object, std::vector<std::vector<std::string>>& texture) {	// set object and texture paths
				myModelPaths = object;
				myTexturePaths = texture;
			}

			virtual std::deque<DMKEventContainer>* events() { return &std::deque<DMKEventContainer>(); }	// get event containers
			virtual bool closeEvent() { return false; }	// get close event

			/* DEFINED FUNCTION(S) */
			void setProgress(uint32_t* progress) { myProgress = progress; }	// set progress
			void setRendererSettings(DMKRendererSettings settings) { myRendererSettings = settings; }	// set renderer settings

		protected:
			DMKRendererSettings myRendererSettings = {};	// renderer settings

			std::deque<DMKEventContainer> myEventContainers = {};	// event containers

			std::vector<std::string> myModelPaths = {};	// model paths
			std::vector<std::vector<std::string>> myTexturePaths = {};	// texture paths
			std::vector<std::string> myShaderPaths = {};	// shader paths

			uint32_t* myProgress = 0;	// progress counter
			bool compileShaders = false;	// compile shaders command
			uint32_t myFormatsCount = 0;	// format counter
		};
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_BASE_H

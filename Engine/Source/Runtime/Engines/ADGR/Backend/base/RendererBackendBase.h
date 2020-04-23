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
 Supported APIs:
 * Vulkan (main)
 * DirectX 12
 * OpenGL
*/

#include <cstdint>
#include <vector>
#include <string>
#include <deque>

#include "Engines/ADGR/Renderer.h"

#define INC_PROGRESS					(*myProgress += 1)
#define MULTIPLE_INC_PROGRESS(count)	for (I32 i_itr__ = 0; i_itr__ < count; i_itr__++) INC_PROGRESS

namespace Dynamik {
	namespace ADGR {
        class RendererBackend {
        public:
            RendererBackend() {}
            virtual ~RendererBackend() {}
        };
	}
}

#endif // !_DYNAMIK_ADGR_RENDERER_BASE_H

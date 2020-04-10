#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_ADGR_PCH
#define _DYNAMIK_ADGR_PCH

#include "DataTypesLib/DataTypesLib.h"
#include "MemoryLib/MemoryLib.h"
#include "SystemLib/SystemLib.h"
#include "AlgorithmLib/AlgorithmLib.h"

#include "CentralDataHub.h"
#include "Managers.h"
#include "Utilities.h"
#include "Platform.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#ifdef DMK_PLATFORM_WINDOWS

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>
#include <functional>
#include <memory>
#include <tuple>
#include <thread>
#include <future>
#include <mutex>
#include <deque>

#include "core/log.h"
#include "CentralDataHub.h"

#if defined(DMK_USE_VULKAN)

#include <GLFW/glfw3.h>

#include <load_ktx.hpp>

#elif defined(DMK_USE_DIRECT_X)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

// The min/max macros conflict with like-named member functions.
// Only use std::min and std::max defined in <algorithm>.
#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// In order to define a function called CreateWindow, the Windows macro needs to
// be undefined.
#if defined(CreateWindow)
#undef CreateWindow
#endif

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include <d3dx12.h>

// STL Headers
#include <algorithm>
#include <cassert>
#include <chrono>

// Helper functions
#include <Helpers.h>

#elif defined(DMK_USE_OPENGL)
#include <GLEW/glew-2.1.0/include/GL/glew.h>
#include <GLFW/glfw3.h>

#endif

#endif

#endif	// !_DYNAMIK_ADGR_PCH

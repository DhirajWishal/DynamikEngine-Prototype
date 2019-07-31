#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifdef DMK_PLATFORM_WINDOWS
#include <iostream>
#include <string>
#include <fstream>

#include <vector>
#include <map>
#include <array>
#include <optional>
#include <functional>
#include <set>

#ifdef DMK_ADGR_USE_VULKAN
#define GLFW_INCLUDE_VULKAN
#define GLFW_DLL
#include <GLFW/glfw3.h>

#elif defined(DMK_ADGR_USE_OPENGL)
#include <GLFW/glfw3.h>
#else

#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#endif

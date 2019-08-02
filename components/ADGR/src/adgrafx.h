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
#include <chrono>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#endif

#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_PCH
#define _DYNAMIK_CENTRAL_DATA_HUB_PCH

#include "DataTypesLib/DataTypesLib.h"
#include "MemoryLib/MemoryLib.h"
#include "SystemLib/SystemLib.h"
#include "AlgorithmLib/AlgorithmLib.h"

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <iostream>
#include <string>
#include <vector>

#endif	// !_DYNAMIK_CENTRAL_DATA_HUB_PCH

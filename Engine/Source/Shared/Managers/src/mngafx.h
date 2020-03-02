#pragma once

#ifndef _DYNAMIK_MANAGERS_PCH
#define _DYNAMIK_MANAGERS_PCH

#include "CentralDataHub.h"
#include "Utilities.h"
#include "Platform.h"

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
#include <fstream>
#include <cstdint>
#include <chrono>
#include <ios>
#include <thread>
#include <future>

#include <string>
#include <vector>
#include <unordered_map>

#endif // !_DYNAMIK_MANAGERS_PCH

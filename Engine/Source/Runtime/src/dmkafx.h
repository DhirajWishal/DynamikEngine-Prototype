#pragma once

#ifndef _DYNAMIK_PCH
#define _DYNAMIK_PCH

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

// ----------
#endif

#endif // 
#endif // !_DYNAMIK_PCH

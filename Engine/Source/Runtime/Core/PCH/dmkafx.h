#pragma once

#ifndef _DYNAMIK_PCH
#define _DYNAMIK_PCH

#include "DataTypesLib.h"
#include "AlgorithmLib.h"
#include "MemoryLib.h"
#include "SystemLib.h"

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
#include <sstream>
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


#if defined(DMK_USE_VULKAN)

#include <GLEW/glew-2.1.0/include/GL/glew.h>
#include <GLFW/glfw3.h>

#include <load_ktx.hpp>

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

// ----------
#endif

#endif // 
#endif // !_DYNAMIK_PCH

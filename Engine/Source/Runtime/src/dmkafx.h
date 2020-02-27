#pragma once

#ifndef _DYNAMIK_PCH
#define _DYNAMIK_PCH

#include "ADGR.h"
#include "Audio.h"
#include "Events.h"
#include "Intellect.h"
#include "Managers.h"
#include "Networking.h"
#include "CentralDataHub.h"
#include "Utilities.h"
#include "Platform.h"

#include <iostream>
#include <stdio.h>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <thread>
#include <optional>

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <deque>

#include <future>

#ifdef DMK_PLATFORM_WINDOWS
#include <fstream>

#include <array>
#include <optional>

#define GLFW_DLL
#include <GLFW/glfw3.h>

#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#ifdef DMK_PLATFORM_WINDOWS
#include <Windows.h>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// ----------
#endif

#endif // !_DYNAMIK_PCH

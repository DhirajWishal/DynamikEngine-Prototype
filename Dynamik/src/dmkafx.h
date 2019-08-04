#pragma once

#include <iostream>
#include <stdio.h>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <ctime>

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <deque>

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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef PLG_PLATFROM_WINDOWS
#include <Windows.h>

// ----------
#endif
#pragma once

#ifndef _DYNAMIK_ADGR_OPENGL_OPENGL_H
#define _DYNAMIK_ADGR_OPENGL_OPENGL_H

#include <GLFW/glfw3.h>

namespace Dynamik {
	namespace ADGR {
		namespace core {
			struct ADGROpenGLDataContainer {
				GLFWwindow* window = nullptr;
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_OPENGL_OPENGL_H

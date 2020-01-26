#include "adgrafx.h"
#include "openglRenderer.h"

namespace Dynamik {
	namespace ADGR {
		openglRenderer::openglRenderer() {
		}

		openglRenderer::~openglRenderer() {
		}

		void openglRenderer::init() {
			myOpenGLDataContainers.resize(openGLDataContainerIndex + 1);

			myWindowManager.init(&myOpenGLDataContainers[openGLDataContainerIndex]);
		}
	}
}
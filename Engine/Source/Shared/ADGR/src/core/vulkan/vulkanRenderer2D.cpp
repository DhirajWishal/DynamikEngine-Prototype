#include "adgrafx.h"
#include "vulkanRenderer2D.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		void vulkanRenderer2D::init() {
		}

		void vulkanRenderer2D::initStageOne() {
		}

		void vulkanRenderer2D::initStageTwo() {
		}

		void vulkanRenderer2D::initStageThree() {
		}

		void vulkanRenderer2D::drawFrame() {
		}

		void vulkanRenderer2D::shutDown() {
		}

		std::deque<DMKEventContainer>* vulkanRenderer2D::events() {
			return nullptr;
		}
		void vulkanRenderer2D::setFormats(ARRAY<RendererFormat>& rendererFormats) {
		}

		void vulkanRenderer2D::updateFormats(ARRAY<RendererFormat>& rendererFormats) {
		}
	}
}
#endif // DEBUG
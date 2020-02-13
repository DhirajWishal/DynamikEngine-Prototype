#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_2D_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_2D_H

#include "core/base/RendererBackendBase.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		class vulkanRenderer2D : public RendererBackend {
		public:
			vulkanRenderer2D() {}
			vulkanRenderer2D(DMKRendererSettings settings) : RendererBackend(settings) {}
			~vulkanRenderer2D() {}

			void init() override;
			void initStageOne() override;
			void initStageTwo() override;
			void initStageThree() override;

			void drawFrame() override;
			void shutDown() override;


			std::deque<DMKEventContainer>* events() override;
			inline B1 closeEvent() override { return true; }

			void setFormats(std::vector<RendererFormat>& rendererFormats) override;
			void updateFormats(std::vector<RendererFormat>& rendererFormats) override;
		};
	}
}
#endif // DMK_USE_VULKAN


#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_2D_H


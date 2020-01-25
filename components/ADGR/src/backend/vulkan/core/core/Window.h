#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_WINDOW_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_WINDOW_H

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Window : public Core {
			public:
				Window() {}
				virtual ~Window() {}

				virtual void pollEvents() {}
				virtual void onUpdate() {}
				virtual bool closeEvent(ADGRVulkanDataContainer* container) { return false; }

				virtual void setWindowIcon(ADGRVulkanDataContainer* container, std::vector<std::string> paths) {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_WINDOW_H

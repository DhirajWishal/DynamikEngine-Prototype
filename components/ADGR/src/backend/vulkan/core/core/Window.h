#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_window_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_window_H

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

				virtual void setwindowIcon(ADGRVulkanDataContainer* container, std::vector<std::string> paths) {}
			};
		}
	}
}

#endif // !_DYNAMIK_ADGR_VULKAN_RENDERER_window_H

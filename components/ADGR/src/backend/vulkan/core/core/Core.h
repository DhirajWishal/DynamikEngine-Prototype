#pragma once

#ifndef _DYNAMIK_VULKAN_RENDERER_CORE_
#define _DYNAMIK_VULKAN_RENDERER_CORE_

#include "backend/vulkan/core/data structures/vulkan.h"

#include "backend/vulkan/core/controlHeader.h"
#include "backend/vulkan/core/data types/typenames.h"
#include "data structures/vertex.h"

#include "backend/vulkan/core/functions/textureFunctions.h"

#include "Event.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class Core {
			public:
				Core() {}
				virtual ~Core() {}

				virtual void initCore() {}
				virtual void drawFrame() {}
				virtual void shutdownCore() {}

				virtual VkInstance getInstance() { return myDataContainer.instance; }
				virtual VkInstance* getInstanceAddr() { return &myDataContainer.instance; }
				virtual VkDevice getDevice() { return myDataContainer.device; }
				virtual VkDevice* getDeviceAddr() { return &myDataContainer.device; }
				virtual VkDebugUtilsMessengerEXT getdebugMessenger() { return myDataContainer.debugMessenger; }
				virtual VkDebugUtilsMessengerEXT* getdebugMessengerAddr() { return &myDataContainer.debugMessenger; }

				virtual void init(ADGRVulkanDataContainer* container) {}
				virtual void clear(ADGRVulkanDataContainer* container) {}

			protected:
				ADGRVulkanDataContainer myDataContainer;
			};
		}
	}
}

#endif

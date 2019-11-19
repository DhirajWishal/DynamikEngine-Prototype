#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_CORE_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_CORE_H

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

				virtual void init(ADGRVulkanDataContainer* container) {}
				virtual void clear(ADGRVulkanDataContainer* container) {}

			protected:
				ADGRVulkanDataContainer myDataContainer;
			};
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_RENDERER_CORE_H

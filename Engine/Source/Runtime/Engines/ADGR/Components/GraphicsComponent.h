#pragma once
#ifndef _DYNAMIK_ADGR_GRAPHICS_COMPONENT_H
#define _DYNAMIK_ADGR_GRAPHICS_COMPONENT_H

#include "Objects/DMKObject/DMKObject.h"
#include "Objects/InternalFormat/InternalFormat.h"
#include "Events/DMKEventComponent.h"

#include "../Backend/Vulkan/vulkanRenderer.h"

namespace Dynamik {
	namespace ADGR {
		class GraphicsComponent : public DMKObject {
		public:
			GraphicsComponent() {}
			virtual ~GraphicsComponent() {}

			virtual void setInternalFormat(POINTER<InternalFormat> format);

			virtual void initialize();
			virtual DMKUniformBufferData onUpdate(DMKCameraData data);
			virtual void terminate();

		protected:
			POINTER<InternalFormat> myInternalFormat;
		};
	}
}

#endif // !_DYNAMIK_ADGR_GRAPHICS_COMPONENT_H

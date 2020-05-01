#include "dmkafx.h"
#include "StaticObject.h"

#include "../../../Backend/Vulkan/VulkanPresets.h"

namespace Dynamik {
	namespace ADGR {
		using namespace Backend;

		void StaticObject::initializeResources()
		{
		}

		void StaticObject::initializeContext()
		{
		}

		DMKUniformBufferData StaticObject::update(DMKCameraData cameraData)
		{
			return DMKUniformBufferData();
		}

		void StaticObject::terminate()
		{
		}
	}
}
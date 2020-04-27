#include "dmkafx.h"
#include "GraphicsComponent.h"

namespace Dynamik {
	namespace ADGR {
		void GraphicsComponent::setInternalFormat(POINTER<InternalFormat> format)
		{
			myInternalFormat = format;
		}
		
		void GraphicsComponent::initialize()
		{
		}

		DMKUniformBufferData GraphicsComponent::onUpdate(DMKCameraData data)
		{
			return myInternalFormat->onUpdate(data);
		}
		
		void GraphicsComponent::terminate()
		{
		}
	}
}
#include "adgrafx.h"
#include "Camera.h"

namespace Dynamik {
	namespace ADGR {
		void Camera::update()
		{
			myLookDirection = glm::normalize(myPosition - myTarget);
		}
	}
}
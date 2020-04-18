#include "dmkafx.h"
#include "applicationEvent.h"

namespace Dynamik {
	std::string WindowResizeEvent::toString() const {
		std::stringstream ss;
		ss << "WindowResizeEvent: " << w_Width << ", " << w_Height;
		return ss.str();
	}
}
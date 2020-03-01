#include "applicationEvent.h"

namespace Dynamik {
	std::wstring WindowResizeEvent::toString() const {
		std::wstringstream ss;
		ss << "WindowResizeEvent: " << w_Width << ", " << w_Height;
		return ss.str();
	}
}
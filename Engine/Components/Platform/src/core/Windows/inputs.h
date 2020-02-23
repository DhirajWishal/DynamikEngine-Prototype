#pragma once

#ifndef _DYNAMIK_PLATFORM_WINDOWS_INPUTS_H
#define _DYNAMIK_PLATFORM_WINDOWS_INPUTS_H

#include "input.h"

namespace Dynamik {
	namespace ADGR {
		namespace windows {
			class windowsInput : public Input {
			protected:
				//virtual bool isKeyPressedImpl(int keyCode) override;
			};
		}
	}
}

#endif // !_DYNAMIK_PLATFORM_WINDOWS_INPUTS_H

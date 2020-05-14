#pragma once
#ifndef _DYNAMIK_CLOCK_H
#define _DYNAMIK_CLOCK_H

#include "Objects/DMKObject/DMKObject.h"
#include <string>

namespace Dynamik {
	class DMKClock : public DMKObject {
		DMKClock() {}
		~DMKClock() {}

		DMK_CLASS_SINGLETON(DMKClock)
		public:
			/* Core functions */
			static void start();
			static D64 update();
			static void end();

			/* Time zone functions */
			static std::string getTimeZoneName();

	private:
		UI64 startTime = 0.0;
	};
}

#endif // !_DYNAMIK_CLOCK_H

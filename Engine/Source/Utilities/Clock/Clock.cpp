#include "dmkafx.h"
#include "Clock.h"

#include <ctime>

namespace Dynamik {
	DMK_CLASS_SINGLETON_IMPL(DMKClock);

	void DMKClock::start()
	{
		time((time_t*)&instance.startTime);
	}

	D64 DMKClock::update()
	{
		return D64();
	}

	void DMKClock::end()
	{
	}

	std::string DMKClock::getTimeZoneName()
	{
		return std::string();
	}
}
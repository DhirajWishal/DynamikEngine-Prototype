#include "dmkafx.h"
#include "Public/Console.h"
#include <iostream>

namespace Dynamik {
	void Console::print(FSTR msg)
	{
		printf(msg.c_str());
	}

	void Console::println(FSTR msg)
	{
		printf("%s\n", msg.c_str());
	}
}
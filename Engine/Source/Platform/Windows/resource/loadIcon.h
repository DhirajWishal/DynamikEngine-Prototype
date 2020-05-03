#pragma once

#ifndef _DYNAMIK_PLATFORM_WINDOWS_LOAD_ICON_H
#define _DYNAMIK_PLATFORM_WINDOWS_LOAD_ICON_H

#include <fstream>
#include <string>

namespace Dynamik {
	std::fstream load_icon(std::string path);
}

#endif // !_DYNAMIK_PLATFORM_WINDOWS_LOAD_ICON_H

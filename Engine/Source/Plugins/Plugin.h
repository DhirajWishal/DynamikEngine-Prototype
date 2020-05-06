#pragma once
#ifndef _DYNAMIK_PLUGIN_H
#define _DYNAMIK_PLUGIN_H

#include <string>
#include "PluginAPI/PluginAPI.h"

namespace Dynamik {
	/* Main Plugin object for the Dynamik Engine.
	 * All plugins are loaded to the engine as shared libraries and imported to the engine at runtime.
	 * These plugins are downloaded from their github repositories and compiled to the library file
	   in the current platform the engine is on. This procedure is done by the Studio.
	 */
	class Plugin  : public DMKPluginAPI {
	public:
		Plugin() {}
		Plugin(std::string path) : myPath(path) {}
		~Plugin() {}

	private:
		std::string myPath = "";
	};
}

#endif // !_DYNAMIK_PLUGIN_H

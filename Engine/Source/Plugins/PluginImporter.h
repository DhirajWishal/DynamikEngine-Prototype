#pragma once
#ifndef _DYNAMIK_PLUGIN_IMPORTER_H
#define _DYNAMIK_PLUGIN_IMPORTER_H

#include "PluginAPI/PluginAPI.h"
#include "Plugin.h"

namespace Dynamik {
	class PluginImporter {
		PluginImporter() {}
		~PluginImporter() {}

	public:
		PluginImporter(const PluginImporter&) = delete;
		PluginImporter(PluginImporter&&) = delete;

		static Plugin& loadPlugin(std::string libraryPath);
	};
}

#endif // !_DYNAMIK_PLUGIN_IMPORTER_H

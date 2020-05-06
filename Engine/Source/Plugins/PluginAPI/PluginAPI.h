#pragma once
#ifndef _DYNAMIK_PLUGIN_API_H
#define _DYNAMIK_PLUGIN_API_H

#include "Objects/InternalFormat/InternalFormat.h"

namespace Dynamik {
	enum class DMKPluginType {
		DMK_PLUGIN_TYPE_WORKSPACE,
		DMK_PLUGIN_TYPE_AUDIO,
		DMK_PLUGIN_TYPE_RENDERING,
	};

	/* Plugin API for the Dynamik Engine.
	 * Clients are able to create plugins for the Dynamik engine using this class as the base.
	 */
	class DMKPluginAPI {
	public:
		DMKPluginAPI() {}
		DMKPluginAPI(DMKPluginType type) : type(type) {}
		virtual ~DMKPluginAPI() {}

		virtual void setInternalFormat(POINTER<InternalFormat> format) {}

		DMKPluginType type = DMKPluginType::DMK_PLUGIN_TYPE_RENDERING;
	};
}

#endif // !_DYNAMIK_PLUGIN_API_H

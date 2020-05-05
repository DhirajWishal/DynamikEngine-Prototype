#pragma once
#ifndef _DYNAMIK_PLUGIN_API_H
#define _DYNAMIK_PLUGIN_API_H

namespace Dynamik {
	/* Plugin API for the Dynamik Engine.
	 * Clients are able to create plugins for the Dynamik engine using this class as the base.
	 */
	class DMKPluginAPI {
	public:
		DMKPluginAPI() {}
		virtual ~DMKPluginAPI() {}
	};
}

#endif // !_DYNAMIK_PLUGIN_API_H

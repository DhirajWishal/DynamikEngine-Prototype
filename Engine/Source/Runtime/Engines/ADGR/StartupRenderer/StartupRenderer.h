#pragma once
#ifndef _DYNAMIK_ADGR_STARTUP_RENDERER_H
#define _DYNAMIK_ADGR_STARTUP_RENDERER_H

namespace Dynamik {
	namespace ADGR {
		class StartupRenderer {
			StartupRenderer() {}

			static StartupRenderer myInstance;

		public:
			StartupRenderer() = delete;
			StartupRenderer(const StartupRenderer&) = delete;
			StartupRenderer(StartupRenderer&&) = delete;

			static void initialize();

		private:

		};
	}
}

#endif // !_DYNAMIK_ADGR_STARTUP_RENDERER_H

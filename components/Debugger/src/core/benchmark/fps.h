#pragma once

#ifndef _DYNAMIK_DEBUGGER_BENCHMARK_FPS_H
#define _DYNAMIK_DEBUGGER_BENCHMARK_FPS_H

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			class FPS {
			public:
				FPS();

				void getFPS();

			private:
				std::chrono::time_point<std::chrono::high_resolution_clock> time;
				std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;

				uint64_t current = 0;
				uint64_t minimum = (uint64_t)10e10;
				uint64_t maximum = 0;
				uint64_t counter = 0;

				bool refresh = false;
			};
		}
	}
}

#endif	// !_DYNAMIK_DEBUGGER_BENCHMARK_FPS_H

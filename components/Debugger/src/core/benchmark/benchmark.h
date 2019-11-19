#pragma once

#ifndef _DYNAMIK_DEBUGGER_BENCHMARK_H
#define _DYNAMIK_DEBUGGER_BENCHMARK_H

#include <chrono>

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			class Benchmark {
			public:
				Benchmark();

				~Benchmark();

			private:
				std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;

				std::chrono::time_point<std::chrono::high_resolution_clock> time;
				std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;
			};
		}
	}
}

#endif // !_DYNAMIK_DEBUGGER_BENCHMARK_H

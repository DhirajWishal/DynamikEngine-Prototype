#include <iostream>
#include <stdio.h>
#include <chrono>
#include <string>

#include "benchmark.h"

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			Benchmark::Benchmark() {
				startTimePoint = std::chrono::high_resolution_clock::now();
			}

			Benchmark::~Benchmark() {
				auto endTimePoint = std::chrono::high_resolution_clock::now();

				auto start = std::chrono::time_point_cast<std::chrono::microseconds>
					(startTimePoint).time_since_epoch().count();
				auto stop = std::chrono::time_point_cast<std::chrono::microseconds>
					(endTimePoint).time_since_epoch().count();

				auto duration = stop - start;
				double milSeconds = duration * .001;

				printf("Timer ended. Results: %lld us ( %f ms)\n", duration, milSeconds);
			}
		}
	}
}
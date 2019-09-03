#pragma once

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

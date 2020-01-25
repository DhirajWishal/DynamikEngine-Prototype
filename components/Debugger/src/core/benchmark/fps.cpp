#include <iostream>
#include <stdio.h>
#include <string>
#include <chrono>

#include "fps.h"

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			FPS::FPS() {
				printf("\t-----Benchmark tool: FPS Calculator initiated!-----\n");
			}

			void FPS::getFPS() {
				if (!refresh) {
					//system("CLS");
					//std::cout << std::flush;
					refresh = true;
				}

				time = std::chrono::high_resolution_clock::now();

				auto start = std::chrono::time_point_cast<std::chrono::microseconds>
					(time).time_since_epoch().count();
				auto old = std::chrono::time_point_cast<std::chrono::microseconds>
					(oldTime).time_since_epoch().count();

				current = (uint64_t)(1000 / ((start - old) * .001));

				if (current < minimum) minimum = current;
				if (current > maximum) maximum = current;

				if (counter == 50) {
					printf("FPS: %I64d\r", current);
					counter = 0;
				}
				else
					counter++;

				oldTime = time;
			}
		}
	}
}
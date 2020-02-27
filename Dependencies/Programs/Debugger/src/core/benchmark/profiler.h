#pragma once

#ifndef _DYNAMIK_DEBUGGER_PROFILER_H
#define _DYNAMIK_DEBUGGER_PROFILER_H

#include <fstream>
#include <string>
#include <chrono>

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			struct benchmarkResult {
				std::string name = "";
				long long start = 0, end = 0;
				uint32_t threadID = 0;
			};

			// Profiler
			class Profiler {
			public:
				Profiler() {}
				~Profiler();

				void beginSession(const std::string& filepath = "profile.json");
				void writeProfile(const benchmarkResult& result);
				void endSession();

				static Profiler& getProfiler();

			private:
				void writeHeader();
				void writeFooter();

				std::ofstream outputFile;
				int profileCount = 0;

				bool isProfilerClosed = false;
			};

			// Profile Timer
			class ProfileTimer {
			public:
				ProfileTimer(std::string name);
				ProfileTimer(std::string name, Profiler* profiler);
				~ProfileTimer();

				void startTimer();
				void stopTimer();

			private:
				Profiler* myProfiler = nullptr;
				std::string myName = "";
				bool isTimerStopped = false;

				std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
			};

			void beginProfiler(std::string fileName);
			void writeToProfiler(const benchmarkResult& result);
			void endProfiler();
		}
	}
}

//#ifdef DMK_DEBUG
#if defined(DMK_DEBUG) || defined(DMK_RELEASE)
#define DMK_DEBUGGER_PROFILER_BEGIN(path)	::Dynamik::Debugger::benchmark::Profiler _##__VA__ARGS__();
#define DMK_DEBUGGER_PROFILER_BEGIN_PATH(path)	::Dynamik::Debugger::benchmark::Profiler::getProfiler().beginSession(path)
#define DMK_DEBUGGER_PROFILER_END ::Dynamik::Debugger::benchmark::Profiler::getProfiler().endSession()

#define DMK_DEBUGGER_SET_FUNCTION_NAME(name, ...) timer_##__VA_ARGS__(name)
#define DMK_DEBUGGER_PROFILER_TIMER_INITIATE(name)	::Dynamik::Debugger::benchmark::ProfileTimer DMK_DEBUGGER_SET_FUNCTION_NAME(name, __LINE__)
#define DMK_DEBUGGER_PROFILER_TIMER_START(...) 	::Dynamik::Debugger::benchmark::ProfileTimer _##__VA_ARGS__(__FUNCSIG__)
#define DMK_DEBUGGER_PROFILER_TIMER_BEGIN(profiler, ...) 	::Dynamik::Debugger::benchmark::ProfileTimer _##__VA_ARGS__(__FUNCSIG__, profiler)
#define DMK_DEBUGGER_PROFILER_TIMER_STOP(...) 	_##__VA_ARGS__.stopTimer()

#else
#define DMK_DEBUGGER_PROFILER_BEGIN(name)
#define DMK_DEBUGGER_PROFILER_BEGIN_PATH(name, path)
#define DMK_DEBUGGER_PROFILER_END

#define DMK_DEBUGGER_SET_FUNCTION_NAME(...)
#define DMK_DEBUGGER_PROFILER_TIMER_INITIATE
#define DMK_DEBUGGER_PROFILER_TIMER_START(...)
#define DMK_DEBUGGER_PROFILER_TIMER_STOP(...)

#endif

#endif

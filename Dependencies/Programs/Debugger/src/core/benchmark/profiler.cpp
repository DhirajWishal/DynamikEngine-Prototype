#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>

#include "profiler.h"

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			std::mutex profilerMutex;

			/* PROFILER */
			Profiler::~Profiler() {
				if (!isProfilerClosed)
					endSession();
			}

			void Profiler::beginSession(const std::string& filepath) {
				outputFile.open(filepath);
				writeHeader();
			}

			void Profiler::writeProfile(const benchmarkResult& result) {
				if (profileCount++ > 0)
					outputFile << ",";

				std::string name = result.name;
				std::replace(name.begin(), name.end(), '"', '\'');

				outputFile << DMK_TEXT("{");
				outputFile << DMK_TEXT("\"cat\":\"function\",");
				outputFile << DMK_TEXT("\"dur\":") << (result.end - result.start) << DMK_TEXT(',');
				outputFile << DMK_TEXT("\"name\":\"") << name.c_str() << DMK_TEXT("\",");
				outputFile << DMK_TEXT("\"ph\":\"X\",");
				outputFile << DMK_TEXT("\"pid\":0,");
				outputFile << DMK_TEXT("\"tid\":") << result.threadID << DMK_TEXT(",");
				outputFile << DMK_TEXT("\"ts\":") << result.start;
				outputFile << DMK_TEXT("}");

				outputFile.flush();
			}

			void Profiler::endSession() {
				if (!isProfilerClosed) {
					writeFooter();
					outputFile.close();
					profileCount = 0;
					isProfilerClosed = true;
				}
			}

			Profiler& Profiler::getProfiler() {
				static Profiler* instance = new Profiler();
				return *instance;
			}

			void Profiler::writeHeader() {
				outputFile << "{\"otherData\": {},\"traceEvents\":[";
				outputFile.flush();
			}

			void Profiler::writeFooter() {
				outputFile << "]}";
				outputFile.flush();
			}

			/* PROFILE TIMER */
			ProfileTimer::ProfileTimer(std::string name) : myName(name) {
				startTimer();
			}

			ProfileTimer::ProfileTimer(std::string name, Profiler* profiler) :
				myName(name), myProfiler(profiler) {
				startTimer();
			}

			void ProfileTimer::startTimer() {
				startTimePoint = std::chrono::high_resolution_clock::now();
			}

			void ProfileTimer::stopTimer() {
				auto endTimePoint = std::chrono::high_resolution_clock::now();

				auto start = std::chrono::time_point_cast<std::chrono::microseconds>
					(startTimePoint).time_since_epoch().count();
				auto stop = std::chrono::time_point_cast<std::chrono::microseconds>
					(endTimePoint).time_since_epoch().count();

				uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
				benchmarkResult results = {};
				results.name = myName;
				results.start = start;
				results.end = stop;
				results.threadID = threadID;

				{
					std::lock_guard<std::mutex> guard(profilerMutex);

					//if (myProfiler == nullptr)
					//	Profiler::getProfiler().writeProfile(results);
					//else
					//	myProfiler->writeProfile(results);

					writeToProfiler(results);
				}

				isTimerStopped = true;
			}

			ProfileTimer::~ProfileTimer() {
				if (!isTimerStopped)
					stopTimer();
			}

			/* ------------------------------------------------------ */

			static std::ofstream profilerOutputFile;
			static int profileCount = 0;

			void beginProfiler(std::string fileName) {
				profilerOutputFile.open(fileName);

				profilerOutputFile << "{\"otherData\": {},\"traceEvents\":[";
				profilerOutputFile.flush();
			}

			void writeToProfiler(const benchmarkResult& result) {
				if (profileCount++ > 0)
					profilerOutputFile << ",";

				std::string name = result.name;
				std::replace(name.begin(), name.end(), '"', '\'');

				profilerOutputFile << DMK_TEXT("{");
				profilerOutputFile << DMK_TEXT("\"cat\":\"function\",");
				profilerOutputFile << DMK_TEXT("\"dur\":") << (result.end - result.start) << DMK_TEXT(',');
				profilerOutputFile << DMK_TEXT("\"name\":\"") << name.c_str() << DMK_TEXT("\",");
				profilerOutputFile << DMK_TEXT("\"ph\":\"X\",");
				profilerOutputFile << DMK_TEXT("\"pid\":0,");
				profilerOutputFile << DMK_TEXT("\"tid\":") << result.threadID << DMK_TEXT(",");
				profilerOutputFile << DMK_TEXT("\"ts\":") << result.start;
				profilerOutputFile << DMK_TEXT("}");

				profilerOutputFile.flush();
			}

			void endProfiler() {
				profilerOutputFile << "]}";

				profilerOutputFile.flush();
				profilerOutputFile.close();
			}
		}
	}
}
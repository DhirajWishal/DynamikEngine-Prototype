#pragma once

#ifndef _DYNAMIK_DEBUGGER_PROFILER_H
#define _DYNAMIK_DEBUGGER_PROFILER_H

#include <fstream>
#include <string>
#include <chrono>

#define DMK_TEXT(...) __VA_ARGS__

namespace Dynamik {
	namespace Debugger {
		namespace benchmark {
			struct ProfileResult
			{
				std::string Name;
				long long Start, End;
				uint32_t ThreadID;
			};

			struct InstrumentationSession
			{
				std::string Name;
			};

			class Profiler
			{
			private:
				InstrumentationSession* m_CurrentSession;
				std::ofstream m_OutputStream;
				int m_ProfileCount;
			public:
				Profiler()
					: m_CurrentSession(nullptr), m_ProfileCount(0)
				{
				}

				void BeginSession(const std::string& name, const std::string& filepath = "results.json");
				void EndSession();
				void WriteProfile(const ProfileResult& result);

				void WriteHeader();
				void WriteFooter();

				static Profiler& Get();
			};

			class ProfileTimer
			{
			public:
				ProfileTimer(const char* name);

				~ProfileTimer();

				void Stop();
			private:
				const char* m_Name;
				std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
				bool m_Stopped;
			};
		}
	}
}

//#ifdef DMK_DEBUG
#if defined(DMK_DEBUG) || defined(DMK_RELEASE)

#define DMK_BEGIN_PROFILING() ::Dynamik::Debugger::benchmark::Profiler::Get().BeginSession(__FUNCSIG__)
#define DMK_BEGIN_PROFILE_TIMER() ::Dynamik::Debugger::benchmark::ProfileTimer timer(__FUNCSIG__)
#define DMK_END_PROFILING() ::Dynamik::Debugger::benchmark::Profiler::Get().EndSession()

#else
#define DMK_BEGIN_PROFILING() 
#define DMK_BEGIN_PROFILE_TIMER() 
#define DMK_END_PROFILING() 

#endif

#endif

#include "dmkafx.h"
#include "Public/ErrorFlag.h"

namespace Dynamik {
	const CHR blue[8] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };	// core info
	const CHR green[8] = { 0x1b, '[', '1', ';', '9', '2', 'm', 0 };	// info
	const CHR yellow[8] = { 0x1b, '[', '1', ';', '9', '3', 'm', 0 };	// warning
	const CHR errRed[8] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };	// error
	const CHR fatalRed[8] = { 0x1b, '[', '4', ';', '3', '1', 'm', 0 };	// fatal
	const CHR normal[8] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };	// default

	enum Calls {
		INFO_L,
		WARN_L,
		ERROR_L,
		FATAL_L,
		CORE_INFO_L
	};

	void changeToColor(int severity) {
		switch (severity) {
		case 1:
			Console::print(green);
			break;
		case 2:
			Console::print(yellow);
			break;
		case 3:
			Console::print(errRed);
			break;
		case 4:
			std::cout << errRed << fatalRed;
			break;
		default:
			Console::print(blue);
			break;
		}
	}

	ARRAY<FSTR> LOG_INFO = {
		"LOG-> ",
		"CORE INFO-> ",
		"CORE WARN-> ",
		"CORE ERROR-> ",
		"CORE FATAL-> ",
		"APPLICATION INFO-> ",
		"APPLICATION WARN-> ",
		"APPLICATION ERROR-> ",
		"APPLICATION FATAL-> ",
	};

	void ErrorFlagSystem::LOG(int severity, FSTR msg, FSTR file, UI32 line) {
		char tmpBuff[128];
		_tzset();

		changeToColor(severity);

		_strtime_s(tmpBuff, 128);
		printf("\n[%s] %s(%s::%u): %s%s",
			tmpBuff,
			LOG_INFO[severity].c_str(),
			file.c_str(),
			line,
			msg.c_str(),
			normal);
	}
	/*
	void ErrorFlagSystem::INFO_LOG(FSTR msg) {
		LOG(INFO_L, msg);
	}

	void ErrorFlagSystem::WARN_LOG(FSTR msg) {
		LOG(WARN_L, msg);
	}

	void ErrorFlagSystem::ERROR_LOG(FSTR msg) {
		LOG(ERROR_L, msg);
	}

	void ErrorFlagSystem::FATAL_LOG(FSTR msg, FSTR file, int line) {
		changeToColor(FATAL_L);

		printf("\n[%s:%u] %s%s%s\n", file.c_str(), line, LOG_INFO[FATAL_L].c_str(), msg.c_str(), normal);
	}

	void ErrorFlagSystem::CORE_LOG(FSTR msg) {
		LOG(CORE_INFO_L, msg);
	}
	*/
	void LOG(int severity, const wchar_t* msg) {
		char tmpBuff[128];
		_tzset();

		changeToColor(severity);

		_strtime_s(tmpBuff, 128);
		printf("\n[%s] %s%s%s\n", tmpBuff, LOG_INFO[severity].c_str(), msg, normal);
	}

	void INFO_LOG(const wchar_t* msg) {
		LOG(INFO_L, msg);
	}

	void WARN_LOG(const wchar_t* msg) {
		LOG(WARN_L, msg);
	}

	void ERROR_LOG(const wchar_t* msg) {
		LOG(ERROR_L, msg);
	}

	void FATAL_LOG(const wchar_t* msg, const wchar_t* file, int line) {
		changeToColor(FATAL_L);

		printf("\n[%s:%u] %s%s%s\n", file, line, LOG_INFO[FATAL_L].c_str(), msg, normal);
	}

	void CORE_LOG(const wchar_t* msg) {
		LOG(CORE_INFO_L, msg);
	}
}
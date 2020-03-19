#include <iostream>
#include <stdio.h>
#include <string>
#include <ctime>
#include <vector>

#include "log.h"

#if defined(DMK_PLATFORM_WINDOWS)

namespace Dynamik {
	const char blue[8] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };		// core info
	const char green[8] = { 0x1b, '[', '1', ';', '9', '2', 'm', 0 };	// info
	const char yellow[8] = { 0x1b, '[', '1', ';', '9', '3', 'm', 0 };	// warning
	const char errRed[8] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };	// error
	const char fatalRed[8] = { 0x1b, '[', '4', ';', '3', '1', 'm', 0 }; // fatal
	const char normal[8] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };	// default

	void changeToColor(int severity) {
		switch (severity) {
		case 0:
			std::cout << green;
			break;
		case 1:
			std::cout << yellow;
			break;
		case 2:
			std::cout << errRed;
			break;
		case 3:
			std::cout << errRed << fatalRed; break;
		case 4:
			std::cout << blue; break;
		default:
			break;
		}
	}

	std::vector<std::string> LOG_INFO = {
		"INFO-> ",
		"WARN-> ",
		"ERROR-> ",
		"FATAL-> ",
		"CORE LOG-> "
	};

	void LOG(int severity, std::string msg) {
		char tmpBuff[128];
		_tzset();

		changeToColor(severity);

		_strtime_s(tmpBuff, 128);
		printf("\n[%s] %s%s%s\n", tmpBuff, LOG_INFO[severity].c_str(), msg.c_str(), normal);
	}

	void INFO_LOG(std::string msg) {
		LOG(INFO_L, msg);
	}

	void WARN_LOG(std::string msg) {
		LOG(WARN_L, msg);
	}

	void ERROR_LOG(std::string msg) {
		LOG(ERROR_L, msg);
	}

	void FATAL_LOG(std::string msg, std::string file, int line) {
		changeToColor(FATAL_L);

		printf("\n[%s:%u] %s%s%s\n", file.c_str(), line, LOG_INFO[FATAL_L].c_str(), msg.c_str(), normal);
	}

	void CORE_LOG(std::string msg) {
		LOG(CORE_INFO_L, msg);
	}

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

// ----------
#endif
#include "dmkafx.h"
#include "log.h"

#if defined(DMK_PLATFORM_WINDOWS)
namespace Dynamik {

	const char blue[8] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };		// core info
	const char green[8] = { 0x1b, '[', '1', ';', '9', '2', 'm', 0 };	// info
	const char yellow[8] = { 0x1b, '[', '1', ';', '9', '3', 'm', 0 };	// warning
	const char errRed[8] = { 0x1b, '[', '1', ';', '3', '1', 'm', 0 };	// error
	const char fatalRed[8] = { 0x1b, '[', '4', ';', '3', '1', 'm', 0 }; // fatal
	const char normal[8] = { 0x1b, '[', '0', ';', '3', '9', 'm', 0 };	// default

	std::vector<std::string> LOG_INFO = {
		"INFO-> ",
		"WARN-> ",
		"ERROR-> ",
		"FATAL-> ",
		"CORE LOG-> "
	};

	void Log::LOG(int severity, std::string msg) {
		char tmpBuff[128];
		_tzset();

		Log::changeToColor(severity);

		_strtime_s(tmpBuff, 128);
		printf("[%s] %s%s%s\n", tmpBuff, LOG_INFO[severity].c_str(), msg.c_str(), normal);

	}

	void Log::INFO_LOG(std::string msg) {
		LOG(INFO_L, msg);
	}

	void Log::WARN_LOG(std::string msg) {
		LOG(WARN_L, msg);
	}

	void Log::ERROR_LOG(std::string msg) {
		LOG(ERROR_L, msg);
	}

	void Log::FATAL_LOG(std::string msg, ccp file, int line) {
		Log::changeToColor(FATAL_L);

		printf("[%s:%u] %s%s%s\n", file, line, LOG_INFO[FATAL_L].c_str(), msg.c_str(), normal);
	}

	void Log::CORE_LOG(std::string msg) {
		LOG(CORE_INFO_L, msg);
	}

	void Log::LOG(int severity, ccp msg) {
		char tmpBuff[128];
		_tzset();

		Log::changeToColor(severity);

		_strtime_s(tmpBuff, 128);
		printf("[%s] %s%s%s\n", tmpBuff, LOG_INFO[severity].c_str(), msg, normal);
	}

	void Log::INFO_LOG(ccp msg) {
		LOG(INFO_L, msg);
	}

	void Log::WARN_LOG(ccp msg) {
		LOG(WARN_L, msg);
	}

	void Log::ERROR_LOG(ccp msg) {
		LOG(ERROR_L, msg);
	}

	void Log::FATAL_LOG(ccp msg, ccp file, int line) {
		Log::changeToColor(FATAL_L);

		printf("[%s:%u] %s%s%s\n", file, line, LOG_INFO[FATAL_L].c_str(), msg, normal);
	}

	void Log::CORE_LOG(ccp msg) {
		LOG(CORE_INFO_L, msg);
	}

	void Log::changeToColor(int severity) {
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
}

// ----------
#endif

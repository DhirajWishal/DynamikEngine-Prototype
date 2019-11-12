#include "dcnsafx.h"
#include "console.h"
#include "core/log.h"

namespace Dynamik {
	namespace console {
		Console::Console() {
		}

		Console::~Console() {
		}

		void Console::write(std::string command) {
			if (command == "help") {
				std::string out = "Help menu.\n\
					help,\n\
					set,\n\
					clear,\n\
					back,\n\
					print.\n";

				printf(out.c_str());
			}

			for (char c : command) {
				if (location == 0 && c != ' ')
					myCommand[COMMAND].push_back(c);
				if (location == 1 && c != ' ')
					myCommand[SUB_COMMAND].push_back(c);
				if (location == 2)
					myCommand[ATTRIBUTES].push_back(c);

				if (location == 0 && c == ' ')
					location++;
				else if (location == 1 && c == ' ')
					location++;

				if (c == '\n')
					break;
			}

			if (myCommand[COMMAND] == commands[SET]) {
			}
			else if (myCommand[COMMAND] == commands[PRINT]) {
				if (myCommand[SUB_COMMAND] == subCommands[ENG_PRINT][0]) {
					DMK_INFO(myCommand[ATTRIBUTES].c_str());
				}
				else if (myCommand[SUB_COMMAND] == subCommands[ENG_PRINT][1]) {
					DMK_WARN(myCommand[ATTRIBUTES].c_str());
				}
				else if (myCommand[SUB_COMMAND] == subCommands[ENG_PRINT][2]) {
					DMK_ERROR(myCommand[ATTRIBUTES].c_str());
				}
				else if (myCommand[SUB_COMMAND] == subCommands[ENG_PRINT][3]) {
					DMK_FATAL(myCommand[ATTRIBUTES].c_str());
				}
				else if (myCommand[SUB_COMMAND] == subCommands[CORE_PRINT][0]) {
					DMK_CORE_MSG(myCommand[ATTRIBUTES].c_str());
				}
				else {
					DMK_CORE_FATAL("Invalid print command");
				}
			}
			else if (myCommand[COMMAND] == commands[CLEAR])
				system("CLS");

			else if (myCommand[COMMAND] == commands[SYSTEM])
				system(("\"" + myCommand[SUB_COMMAND] + " " + myCommand[ATTRIBUTES] + "\"").c_str());

			else if (myCommand[COMMAND] == commands[SET]) {
			}
			else if (myCommand[COMMAND] == commands[GET]) {
			}
			else if (myCommand[COMMAND] == commands[BACK]) {
			}
		}

		void Console::print() {
		}
	}
}
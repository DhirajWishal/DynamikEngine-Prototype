#pragma once

#ifndef _DYNAMIK_DEVCON_CONSOLE_H
#define _DYNAMIK_DEVCON_CONSOLE_H

namespace Dynamik {
	namespace console {
		class Console {
		public:
			Console();
			~Console();

			void write(std::string command);

		private:
			void print();

			std::vector<std::string> myCommand = { {}, {}, {}, {} };
			int location = 0;

			enum format {
				COMMAND,
				SUB_COMMAND,
				ATTRIBUTES,
				FLAGS
			};

			enum com {
				HELP, SET, CLEAR,
				BACK, GET, PRINT,
				SYSTEM
			};

			const std::vector<std::string> commands = {
				"help", "set", "clear", "back",
				"get", "print", "system"
			};

			enum subCom {
				ENG_PRINT,
				CORE_PRINT
			};

			const std::vector<std::vector<std::string>> subCommands = {
				{"info", "warn", "error", "fatal"},
				{"corelog"},
			};
		};
	}
}

#endif // !_DYNAMIK_DEVCON_CONSOLE_H

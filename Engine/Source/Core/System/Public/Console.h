#pragma once
#ifndef _DYNAMIK_SYSTEM_CONSOLE_H
#define _DYNAMIK_SYSTEM_CONSOLE_H

#include "Public/FastString.h"
#include <vector>
#include "Public/Dynamic.h"

#define TO_CONSOLE(x,...) printf(x, ##__VA_ARGS__)

namespace Dynamik {
	class Console {
	public:
		Console() {}
		~Console() {}

		static void print(FSTR msg);
		static void println(FSTR msg);

		template<class TYPE>
		static void print(std::vector<TYPE> arr, CHR end = '\n')
		{
			for (TYPE element : arr)
				std::cout << element << end;
		}

		/* FORMATTING
		 [UI8]	- unsigned 8bit integer value.
		 [UI16] - unsigned 16bit integer value.
		 [UI32] - unsigned 32bit integer value.
		 [UI64] - unsigned 64bit integer value.
		 [I8]	- 8bit integer value.
		 [I16]	- 16bit integer value.
		 [I32]	- 32bit integer value.
		 [I64]	- 64bit integer value.
		 [SI8]	- signed 8bit integer value.
		 [SI16] - signed 16bit integer value.
		 [SI32] - signed 32bit integer value.
		 [SI64] - signed 64bit integer value.
		 [B1]	- 4bit boolean value.
		 [F32]	- 32bit float value.
		 [D64]	- 64bit double value.
		 [PTR]	- pointer value(address).
		 [CHR]	- 4bit char value.
		 [WCHR]	- wide char value.
		 [ARR]	- array value.
		*/
		template<class... ARGS>
		static void print(FSTR format, ARGS... arguments)
		{
			//std::vector<DYNAMIC> _myDataStore;
			B1 isInFormat = false;
			std::string _myFinalString;
			for (UI32 _itr = 0; _itr < format.size(); _itr++)
			{
				if (format[_itr] == '[')
				{
					std::string _myFormatString;
					while (format[++_itr] != ']')
						_myFormatString.push_back(format[_itr]);

					if (_myFormatString == "UI8")
						_myFinalString.append("%u ");
					else if (_myFormatString == "UI16")
						_myFinalString.append("%u ");
					else if (_myFormatString == "UI32")
						_myFinalString.append("%u ");
					else if (_myFormatString == "UI64")
						_myFinalString.append("%u ");
					else if (_myFormatString == "I8")
						_myFinalString.append("%d ");
					else if (_myFormatString == "I16")
						_myFinalString.append("%d ");
					else if (_myFormatString == "I32")
						_myFinalString.append("%d ");
					else if (_myFormatString == "I64")
						_myFinalString.append("%d ");
					else if (_myFormatString == "SI8")
						_myFinalString.append("%i ");
					else if (_myFormatString == "SI16")
						_myFinalString.append("%i ");
					else if (_myFormatString == "SI32")
						_myFinalString.append("%i ");
					else if (_myFormatString == "SI64")
						_myFinalString.append("%i ");
					else if (_myFormatString == "B1")
						_myFinalString.append("%i ");
					else if (_myFormatString == "F32")
						_myFinalString.append("%f ");
					else if (_myFormatString == "D64")
						_myFinalString.append("%g ");
					else if (_myFormatString == "PTR")
						_myFinalString.append("%p ");
					else if (_myFormatString == "CHR")
						_myFinalString.append("%s ");
					else if (_myFormatString == "WCHR")
						_myFinalString.append("%s ");
				}
			}

			_myFinalString.append("\n");
			//TO_CONSOLE(_myFinalString, __VA_ARGS__);
		}
	};
}

#endif // !_DYNAMIK_SYSTEM_CONSOLE_H

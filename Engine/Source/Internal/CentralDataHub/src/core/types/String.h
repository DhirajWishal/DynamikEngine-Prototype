#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_STRING_H
#define _DYNAMIK_CENTRAL_DATA_HUB_STRING_H

#include "core/global/datatypes.h"
#include "Platform/Windows/memory/oneTimeAllocator.h"
#include "Array.h"
#include <string>

#define DMK_STRING_MATCH_NOT_FOUND	-1
#define DMK_STRING_INVALID_INDEX	'\0'

namespace Dynamik {
	/*
	 Private String datatype for the Dynamik Engine.
	*/
	class STRING {
	public:
		STRING() {}
		STRING(std::string string) : myStringData(string.data()), myStringSize(string.size()) {}
		STRING(CCPTR string) : myStringData((CPTR)string), myStringSize(_getCCPTRSize(string)) {}
		STRING(CPTR string) : myStringData(string), myStringSize(_getCCPTRSize(string)) {}
		~STRING() { myStringData = nullptr; myStringSize = 0; }

		/* FUNCTIONS */
		CCPTR c_str();
		UI32 size();
		void add(CHR chr);
		void append(STRING string);

		/* OPERATOR OVERLOADING */
		STRING operator=(STRING& string);
		STRING operator=(CPTR& string);
		bool operator==(STRING& string);
		CHR operator[](UI32 index);

		STRING operator+(STRING string);
		STRING operator+(CCPTR string);

	private:
		UI32 _getCCPTRSize(CCPTR string);

		CPTR myStringData = nullptr;
		UI32 myStringSize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_STRING_H

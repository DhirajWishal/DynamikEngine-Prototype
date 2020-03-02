#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_STRING_H
#define _DYNAMIK_CENTRAL_DATA_HUB_STRING_H

#include "core/global/datatypes.h"
#include "core/types/Array.h"
#include <string>

#define DMK_STRING_MATCH_NOT_FOUND	-1
#define DMK_STRING_INVALID_INDEX	'\0'

namespace Dynamik {
	/*
	 Private String datatype for the Dynamik Engine.
	*/
	class String {
	public:
		String() {}
		String(std::string string) : myStringData(string.data()), myStringSize(string.size()) {}
		String(CCPTR string) : myStringData((CPTR)string), myStringSize(_getCCPTRSize(string)) {}
		String(CPTR string) : myStringData(string), myStringSize(_getCCPTRSize(string)) {}
		~String() { myStringData = nullptr; myStringSize = 0; }

		/* FUNCTIONS */
		CCPTR c_str();
		UI32 size();
		void add(CHR chr);
		void append(String string);

		/* OPERATOR OVERLOADING */
		String operator=(String& string);
		String operator=(CPTR& string);
		bool operator==(String& string);
		CHR operator[](UI32 index);

		String operator+(String string);
		String operator+(CCPTR string);

	private:
		UI32 _getCCPTRSize(CCPTR string);

		CPTR myStringData = nullptr;
		UI32 myStringSize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_STRING_H

#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_FAST_STRING_H
#define _DYNAMIK_CENTRAL_DATA_HUB_FAST_STRING_H

#include "core/global/datatypes.h"
#include "core/types/String.h"
#include "core/types/Array.h"
#include <string>

namespace Dynamik {
	/*
	 Fast String datatype for the Dynamik Engine
	*/
	class FastString {
	public:
		FastString() {}	// default constructor
		FastString(std::string string) : myString((CPTR)string.c_str()), mySize(string.size()) {}	// default constructor
		FastString(String string) : myString((CPTR)string.c_str()), mySize(string.size()) {}	// default constructor
		FastString(CPTR string) : myString(string), mySize(_getSizeCPTR(string)) {}	// default constructor
		~FastString() {}	// default destructor

		UI32 size();	// return the size of the string

		I32 find(CHR chr);	// find a character
		I32 find(String string);	// find a string

		CCPTR c_str();	// convert to const char*
		String s_str();	// convert to String
		std::string str();	// convert to std::string

		VPTR data();	// return the address of the data stored

		void clear();	// clear all data in the container
		void replace(UI32 index, CHR chr);	// replace a character

		FastString operator=(std::string string);	// = operator overload
		FastString operator=(String string);	// = operator overload
		FastString operator=(CPTR string);	// = operator overload
		bool operator==(FastString string);	// == operator overload
		CHR operator[](UI32 index);	// [] operator overload

	private:
		UI32 _getSizeCPTR(CPTR string);	// get the size of a CPTR string
		I32 _findChr(CHR chr);	// find a character in the string

		CPTR myString = nullptr;	// string container
		UI32 mySize = 0;	// string size container
	};

	class FastWideString {
	public:
		FastWideString() {}
		FastWideString(WCPTR string) : myString(string), mySize() {}
		~FastWideString() {}

	private:
		WCPTR myString = nullptr;
		UI32 mySize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_FAST_STRING_H

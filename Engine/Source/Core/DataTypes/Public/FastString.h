#pragma once
#ifndef _DYNAMIK_DATA_TYPES_FAST_STRING_H
#define _DYNAMIK_DATA_TYPES_FAST_STRING_H

#include "datatypes.h"
#include "String.h"

namespace Dynamik {
	class FWSTR;

	/*
	 * Fast String datatype for the Dynamik Engine
	 */
	class FSTR {
	public:
		// default constructor
		FSTR() {}
		// default constructor
		FSTR(CPTR string) : myString(string), mySize(_getSizeCPTR(string)) {}
		// default constructor
		FSTR(CCPTR string) : myString((CPTR)string), mySize(_getSizeCPTR((CPTR)string)) {}
		// default destructor
		~FSTR() {}

		// return the size of the string
		UI32 size();

		// find a character
		I32 find(CHR chr);
		// find a string
		I32 find(FSTR string);

		CCPTR& c_str();	// convert to const char*
		std::string& str();	// convert to std::string

		VPTR data();	// return the address of the data stored

		void clear();	// clear all data in the container
		void replace(UI32 index, CHR chr);	// replace a character

		FSTR operator=(std::string string);	// = operator overload
		FSTR operator=(CCPTR string);	// = operator overload
		FSTR operator=(CPTR string);	// = operator overload
		FSTR operator=(FWSTR wstring);	// = operator overload (for wide string)
		bool operator==(FSTR string);	// == operator overload
		CHR operator[](UI32 index);	// [] operator overload

	private:
		UI32 _getSizeCPTR(CPTR string);	// get the size of a CPTR string
		I32 _findChr(CHR chr);	// find a character in the string

		CPTR myString = nullptr;	// string container
		UI32 mySize = 0;	// string size container
	};

	/*
	 Fast String (UTF-8) datatype for the Dynamik Engine
	*/
	class FWSTR {
	public:
		FWSTR() {}
		FWSTR(WCPTR string) : myString(string), mySize() {}
		~FWSTR() {}

	private:
		WCPTR myString = nullptr;
		UI32 mySize = 0;
	};
}
#endif // !_DATA_TYPES_FAST_STRING_H

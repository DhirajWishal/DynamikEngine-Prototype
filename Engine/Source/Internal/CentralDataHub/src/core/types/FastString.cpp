#include "cdhafx.h"
#include "FastString.h"
#include "core/log.h"

namespace Dynamik {
	UI32 FastString::size()
	{
		return mySize;
	}

	I32 FastString::find(CHR chr)
	{
		return _findChr(chr);
	}

	I32 FastString::find(String string)
	{
		B1 _found = false;
		UI32 _itr = 0;
		while ((_itr < mySize)) {
			if (myString[_itr] == string[0]) {
				if (mySize - _itr < string.size())
					break;
				for (UI32 _i = 0; _i < string.size(); _i++) {
					if (myString[_itr + _i] != string[_i]) {
						_found = false;
						break;
					}
					else {
						_found = true;
					}
				}

				if (_found)
					return _itr;
			}
		}

		return DMK_STRING_MATCH_NOT_FOUND;
	}

	CCPTR FastString::c_str()
	{
		return myString;
	}

	String FastString::s_str()
	{
		return String(myString);
	}

	std::string FastString::str()
	{
		return std::string(myString);
	}

	VPTR FastString::data()
	{
		return &myString;
	}

	void FastString::clear()
	{
		myString = nullptr;
	}

	void FastString::replace(UI32 index, CHR chr)
	{
		if (index >= mySize) {
			DMK_CORE_FATAL("Fast String Replace Index Out Of Range!");
			return;
		}

		myString[index] = chr;
	}

	FastString FastString::operator=(std::string string)
	{
		this->myString = (CPTR)string.c_str();
		this->mySize = string.size();
		return *this;
	}

	FastString FastString::operator=(String string)
	{
		this->myString = (CPTR)string.c_str();
		this->mySize = string.size();
		return *this;
	}

	FastString FastString::operator=(CPTR string)
	{
		this->myString = string;
		this->mySize = this->_getSizeCPTR(string);
		return *this;
	}

	bool FastString::operator==(FastString string)
	{
		if (string.size() != this->mySize)
			return false;
		for (UI32 _itr = 0; _itr < this->mySize; _itr++)
			if (string[_itr] != this->myString[_itr])
				return false;

		return true;
	}

	CHR FastString::operator[](UI32 index)
	{
		if (index >= mySize)
			return DMK_STRING_INVALID_INDEX;

		return myString[index];
	}

	UI32 FastString::_getSizeCPTR(CPTR string)
	{
		UI32 _size = 0;
		while (string[_size] != '\0') _size++;
		return _size;
	}

	I32 FastString::_findChr(CHR chr)
	{
		UI32 _itr = 0;
		while (_itr < mySize) {
			if (myString[_itr] == chr)
				return _itr;
		}

		return DMK_STRING_MATCH_NOT_FOUND;
	}
}
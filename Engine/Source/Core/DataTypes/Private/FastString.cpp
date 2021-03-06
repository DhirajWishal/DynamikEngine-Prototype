#include "dmkafx.h"
#include "Public/FastString.h"
#include <codecvt>

namespace Dynamik {
	UI32 FSTR::size()
	{
		return mySize;
	}

	I32 FSTR::find(CHR chr)
	{
		return _findChr(chr);
	}

	I32 FSTR::find(FSTR string)
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

	CCPTR& FSTR::c_str()
	{
		return (CCPTR&)myString;
	}

	std::string& FSTR::str()
	{
		return std::string(myString);
	}

	VPTR FSTR::data()
	{
		return &myString;
	}

	void FSTR::clear()
	{
		myString = nullptr;
	}

	void FSTR::replace(UI32 index, CHR chr)
	{
		if (index >= mySize) {
			return;
		}

		myString[index] = chr;
	}

	FSTR FSTR::operator=(std::string string)
	{
		this->myString = (CPTR)string.c_str();
		this->mySize = string.size();
		return *this;
	}

	FSTR FSTR::operator=(CCPTR string)
	{
		this->myString = (CPTR)string;
		this->mySize = this->_getSizeCPTR((CPTR)string);
		return *this;
	}

	FSTR FSTR::operator=(CPTR string)
	{
		this->myString = string;
		this->mySize = this->_getSizeCPTR(string);
		return *this;
	}

	FSTR FSTR::operator=(FWSTR wstring)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		this->myString = (CPTR)converterX.to_bytes(std::wstring()).c_str();	// include wstring.s_str()
		return *this;
	}

	bool FSTR::operator==(FSTR string)
	{
		if (string.size() != this->mySize)
			return false;
		for (UI32 _itr = 0; _itr < this->mySize; _itr++)
			if (string[_itr] != this->myString[_itr])
				return false;

		return true;
	}

	CHR FSTR::operator[](UI32 index)
	{
		if (index >= mySize)
			return DMK_STRING_INVALID_INDEX;

		return myString[index];
	}

	UI32 FSTR::_getSizeCPTR(CPTR string)
	{
		UI32 _size = 0;
		while (string[_size] != '\0') _size++;
		return _size;
	}

	I32 FSTR::_findChr(CHR chr)
	{
		UI32 _itr = 0;
		while (_itr < mySize) {
			if (myString[_itr] == chr)
				return _itr;
		}

		return DMK_STRING_MATCH_NOT_FOUND;
	}
}
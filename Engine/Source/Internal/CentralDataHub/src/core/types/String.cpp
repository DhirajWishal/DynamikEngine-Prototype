#include "cdhafx.h"
#include "String.h"
#include "core/log.h"

#include "Platform/Windows/memory/memoryManager.h"

namespace Dynamik {
	CCPTR String::c_str()
	{
		return myStringData;
	}
	UI32 String::size()
	{
		return this->myStringSize;
	}

	void String::add(CHR chr)
	{
		const UI32 _size = 1;
		UI32* _cont = (UI32*)&_size;
		*_cont = myStringSize + 1;
		manager::OneTimeAllocator once(_size);
		CPTR _buffer = (CPTR)once.getAddress();

		//memset(_buffer, ' ', _size);

		UI32 _itr = 0;
		while (_itr < myStringSize) { _buffer[_itr] = myStringData[_itr]; _itr++; }
		_buffer[_itr] = chr;

		myStringSize++;
		myStringData = _buffer;
		delete _buffer;
	}

	void String::append(String string)
	{
		const UI32 _size = 1;
		UI32* _cont = (UI32*)&_size;
		*_cont = myStringSize + string.size() - 1;
		CHR _buffer[_size] = { "" };

		I32 _itr = -1;
		while (_itr++ != myStringSize) _buffer[_itr] = myStringData[_itr];
		myStringSize += string.size() - 1;
		UI32 _subCount = string.size();
		while (_itr++ != myStringSize) _buffer[_itr] = string[_subCount - _itr];

		myStringData = _buffer;
	}

	CHR String::operator[](UI32 index)
	{
		if (index > this->myStringSize - 1)
			DMK_CORE_FATAL("String Index Out Of Range!");

		return this->myStringData[index];
	}

	String String::operator+(String string)
	{
		this->append(string);
		return *this;
	}

	String String::operator+(CCPTR string)
	{
		this->append(string);
		return *this;
	}

	UI32 String::_getCCPTRSize(CCPTR string)
	{
		UI32 _size = 0;
		while (string[_size] != '\0') _size++;
		return _size;
	}

	String String::operator=(String& string)
	{
		this->myStringData = string.myStringData;
		this->myStringSize = string.size();
		return *this;
	}

	String String::operator=(CPTR& string)
	{
		this->myStringData = string;
		this->myStringSize = this->_getCCPTRSize(string);
		return *this;
	}

	bool String::operator==(String& string)
	{
		if (string.size() != this->myStringSize)
			return false;
		for (UI32 _itr = 0; _itr < this->myStringSize; _itr++)
			if (string[_itr] != this->myStringData[_itr])
				return false;

		return true;
	}
}
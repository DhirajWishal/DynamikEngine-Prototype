#include "DataTypesLib/Public/String.h"
#include "MemoryLib/Public/oneTimeAllocator.h"

namespace Dynamik {
	// Default constructor
	STRING::STRING(std::string string)
	{
		//if (string.size() < (MEMORY_ALIGN * MEMORY_ALIGN))
		if (false)
			_initStackAllocatedString(string.data());	// Store it in the pre defined buffer
		else
			_reallocatePushStr(string.data());	// Reallocate data and push it to the main store.
	}

	// Default constructor
	STRING::STRING(CCPTR string)
	{
		// Reallocate data and push it to the main store.
		_reallocatePushStr(string);
	}

	// Default constructor
	STRING::STRING(CPTR string)
	{
		// Reallocate data and push it to the main store.
		_reallocatePushStr(string);
	}

	// Default destructor
	STRING::~STRING()
	{
		// check if the main data store is deallocated or not
		if (myStringData.isValid()) {
			// if not deallocated, clear and destroy pointers and then deallocate.
			_clearAndDestroyPointers();
			Allocator::deAllocate(myStringData, myAllocatedSize);
		}

		// Set all pointers and variables to nullptr and 0;
		myStringData = nullptr;
		myEndPtr = nullptr;
		myAllocatedSize = 0;
		myStringSize = 0;
	}

	// Convert the string to const CHR*
	CCPTR STRING::c_str()
	{
		return myStringData;
	}

	// Convert the string to std::string
	std::string STRING::s_str()
	{
		return myStringData;
	}

	// Get the size of the string (character count)
	UI32 STRING::size()
	{
		return myStringSize;
	}

	// Push a character to the end of the string
	void STRING::push(CHR chr)
	{
		// Reallocate memory and add the character to the end
		_reallocatePushChr(chr);
	}

	// Append a string to the end of this
	void STRING::append(STRING string)
	{
		// Reallocate memory and append the string
		_reallocatePushStr(string.data());
	}

	// Return the character at a given index
	CHR& STRING::at(I32 index)
	{
		// Check if the provided index is valid and flags an error if its not valid
		if (index >= myStringSize || (index <= (0 - myStringSize))); /* TODO: Error flagging. */

		if (index < 0)
		{
			index *= (-1);
			index = (myStringSize - index);
		}

		return myStringData[index];
	}

	// return the allocation size of the string
	UI32 STRING::capacity()
	{
		return myAllocatedSize;
	}

	// Find a given character in the string
	UI32 STRING::find(CHR chr)
	{
		UI32 _itr = 0;
		while (_itr < myStringSize) {
			if (myStringData[_itr] == chr)
				return _itr;
		}

		// return -1 if not found
		return DMK_STRING_MATCH_NOT_FOUND;
	}

	// Find a given string in this.
	UI32 STRING::find(STRING string)
	{
		B1 _found = false;
		UI32 _itr = 0;
		while ((_itr < myStringSize)) {
			if (myStringData[_itr] == string[0]) {
				if (myStringSize - _itr < string.size())
					break;
				for (UI32 _i = 0; _i < string.size(); _i++) {
					if (myStringData[_itr + _i] != string[_i]) {
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

		// return -1 if not found
		return DMK_STRING_MATCH_NOT_FOUND;
	}

	// Return the address of the store
	POINTER<CHR> STRING::data()
	{
		return myStringData;
	}

	// Check if a given index is valid
	B1 STRING::isIndexValid(UI32 index)
	{
		return (index < myStringSize);
	}

	// Begin iterator of the string
	POINTER<CHR> STRING::begin()
	{
		return myBeginPtr;
	}

	// End iterator of the string
	POINTER<CHR> STRING::end()
	{
		return myEndPtr;
	}

	// Return the character at a given index
	CHR& STRING::operator[](I32 index)
	{
		// Calls the at(UI32) function to return the value
		return this->at(index);
	}

	// Append another string
	STRING STRING::operator+(STRING string)
	{
		this->append(string);
		return *this;
	}

	// Append another string
	STRING STRING::operator+(CCPTR string)
	{
		this->append(string);
		return *this;
	}

	// Return the size of a raw string (const CHR*)
	UI32 STRING::_getCCPTRSize(CCPTR string)
	{
		UI32 _size = 0;
		while (string[_size] != '\0') _size++;
		return _size;
	}

	// Clear and destroy pointers
	inline void STRING::_clearAndDestroyPointers()
	{
		setData(myStringData, ' ', myStringSize);

		myStringData.~POINTER();
		myBeginPtr.~POINTER();
		myEndPtr.~POINTER();
	}

	// Reallocate and push a char to the end
	void STRING::_reallocatePushChr(CHR chr)
	{
		POINTER<CHR> _newString = Allocator::allocate(myAllocatedSize + 1);
		POINTER<CHR> _elementPointer = _newString;
		_elementPointer += myStringSize;
		Allocator::set(_elementPointer, (CHR&&)chr);

		if (myStringData.isValid())
		{
			moveBytes(_newString, myStringData, myStringSize);

			_clearAndDestroyPointers();
			Allocator::deAllocate(myStringData, myAllocatedSize);
		}

		myStringData = _newString;
		myBeginPtr = _newString;
		myEndPtr = _elementPointer;
		myAllocatedSize++;
		myStringSize++;
	}

	// Reallocate and append a string
	void STRING::_reallocatePushStr(POINTER<CHR> string)
	{
		UI32 _stringSize = _getCCPTRSize(string);
		UI32 _newSizeToAllocate = (((myAllocatedSize + _stringSize) >= 4) ? ((_stringSize + myAllocatedSize) - 4) : (_stringSize + myAllocatedSize));
		POINTER<CHR> _newString = Allocator::allocate(_newSizeToAllocate);
		POINTER<CHR> _elementPointer = _newString;
		_elementPointer += myStringSize;
		moveBytes(_elementPointer, string, _stringSize);

		if (myStringData.isValid())
		{
			moveBytes(_newString, myStringData, myAllocatedSize);

			_clearAndDestroyPointers();
			Allocator::deAllocate(myStringData, myAllocatedSize);
		}

		myStringData = _newString;
		myStringData[_stringSize] = 0;
		myBeginPtr = _newString;
		myEndPtr = _elementPointer;
		myEndPtr += _stringSize;
		myAllocatedSize = _newSizeToAllocate;
		myStringSize += _stringSize;
	}

	// Initialize the stack allocated string
	inline void STRING::_initStackAllocatedString(POINTER<CHR> string)
	{
		/* TODO */
	}

	// Copy the stack allocated string to the main data store
	inline void STRING::_copyStackAllocatedStringToMain()
	{
		/* TODO */
	}

	// Assign another string to this
	STRING STRING::operator=(STRING& string)
	{
		this->myStringData = string.myStringData;
		this->myStringSize = string.size();
		return *this;
	}

	// Assign another string to this
	STRING STRING::operator=(CPTR& string)
	{
		this->myStringData = string;
		this->myStringSize = this->_getCCPTRSize(string);
		return *this;
	}

	// Check if this and a given string are equal
	B1 STRING::operator==(STRING& string)
	{
		if (string.size() != this->myStringSize)
			return false;
		for (UI32 _itr = 0; _itr < this->myStringSize; _itr++)
			if (string[_itr] != this->myStringData[_itr])
				return false;

		return true;
	}

	// Check if this and a given string are equal
	B1 STRING::operator==(CCPTR string)
	{
		STRING _local = string;
		if (_local.size() != this->myStringSize)
			return false;
		for (UI32 _itr = 0; _itr < this->myStringSize; _itr++)
			if (_local[_itr] != this->myStringData[_itr])
				return false;

		return true;
	}
}
#pragma once
#ifndef _DYNAMIK_DATA_TYPES_STRING_H
#define _DYNAMIK_DATA_TYPES_STRING_H

#include "datatypes.h"
#include "Public/StaticAllocator.h"
#include "Array.h"
#include <string>

namespace Dynamik {
	/*
	 * Private String datatype.
	 */
	class STRING {
		/* DEFAULTS */

		/* PRIVATE DATATYPE
		 * Internal Allocator datatype.
		 * Used to store string data in the heap.
		 */
		typedef StaticAllocator<CHR> Allocator;

	public:
		/* PUBLIC DATAYPE
		 * Iterator for data in the string.
		 */
		typedef POINTER<CHR> ITERATOR;

	public:
		/* CONSTRUCTOR
		 * Default constructor.
		 */
		STRING() {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param string: std::string value.
		 */
		STRING(std::string string);

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param string: const CHR* value.
		 */
		STRING(CCPTR string);

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param string: CHR* value.
		 */
		STRING(CPTR string);

		/* DESTRUCTOR
		 * Default destructor.
		 */
		~STRING();

		/* PUBLIC FUNCTIONS */
	public:
		/* FUNCTION
		 * Push a character to the end of the string.
		 *
		 * @param chr: Character to be inserted to the end.
		 */
		void push(CHR chr);

		/* FUNCTION
		 * Append a string to the end of the string.
		 *
		 * @param string: String to be inserted to the end.
		 */
		void append(STRING string);

		/* FUNCTION
		 * Access an element/ character at a given index.
		 * Negative indexing is possible.
		 *
		 * @param index: Index to be accessed within the string.
		 */
		CHR& at(I32 index = 0);

		/* FUNCTION
		 * Convert the string to a const CHR*.
		 */
		CCPTR c_str();

		/* FUNCTION
		 * Convert the string to a std::string type.
		 */
		std::string s_str();

		/* FUNCTION
		 * Return the size of the string (number of characters stored.
		 */
		UI32 size();

		/* FUNCTION
		 * Return the size of the allocation.
		 * capacity - size = the available space till the next allocation.
		 */
		UI32 capacity();

		/* FUNCTION
		 * Find a character in the string and return its index.
		 * Returns STRING_MATCH_NOT_FOUND if its not fount.
		 *
		 * @param chr: Character to search.
		 */
		UI32 find(CHR chr);

		/* FUNCTION
		 * Find a matching string in the string and return its first character's index.
		 * Returns STRING_MATCH_NOT_FOUND if its not fount.
		 *
		 * @param string: String to search for.
		 */
		UI32 find(STRING string);

		/* FUNCTION
		 * Return the data pointer of the store.
		 */
		POINTER<CHR> data();

		/* FUNCTION
		 * Checks if a given index is valid.
		 *
		 * @param index: Index to be checked.
		 */
		B1 isIndexValid(UI32 index = 0);

		/* FUNCTION
		 * Begin iterator of the string.
		 */
		POINTER<CHR> begin();

		/* FUNCTION
		 * End iterator of the string.
		 */
		POINTER<CHR> end();

		/* OPERATOR OVERLOADING */
	public:
		/* OPERATOR
		 * Assigns another string to this.
		 *
		 * @param string: A string with the data type of STRING.
		 */
		STRING operator=(STRING& string);

		/* OPERATOR
		 * Assigns another string to this.
		 *
		 * @param string: A string with the data type of CHR*.
		 */
		STRING operator=(CPTR& string);

		/* OPERATOR
		 * Check if the given string is equal to this.
		 *
		 * @param string: A string with the data type of STRING.
		 */
		B1 operator==(STRING& string);

		/* OPERATOR
		 * Check if the given string is equal to this.
		 *
		 * @param string: A string with the data type of const CHR*.
		 */
		B1 operator==(CCPTR string);

		/* OPERATOR
		 * Return the character in the provided index.
		 * Negative indexing is accepted.
		 *
		 * @param index: The index to be accessed.
		 */
		CHR& operator[](I32 index);

		/* OPERATOR
		 * Add another string to this (append).
		 *
		 * @param string: A string of the STRING data type.
		 */
		STRING operator+(STRING string);

		/* OPERATOR
		 * Add another string to this (append).
		 *
		 * @param string: A string of the const CHR* data type.
		 */
		STRING operator+(CCPTR string);

		/* INTERNAL FUNCTIONS */
	private:
		/* FUNCTION
		 * Get the size of a const CHR* string.
		 *
		 * @param string: A string of the const CHR* data type.
		 */
		inline UI32 _getCCPTRSize(CCPTR string);

		/* FUNCTION
		 * Clear the data in the memory and destroys the pointers.
		 * Calls the destructors of the pointers.
		 */
		inline void _clearAndDestroyPointers();

		/* FUNCTION
		 * Reallocate memory and push a character to the end.
		 *
		 * @param chr: Character to be added.
		 */
		inline void _reallocatePushChr(CHR chr);

		/* FUNCTION
		 * Reallocate memory and concatenate a string to the end.
		 *
		 * @param string: String to be appended to this.
		 */
		inline void _reallocatePushStr(POINTER<CHR> string);

		/* FUNCTION
		 * Initialize the stack allocated string.
		 *
		 * @param string: The string to be initialized.
		 */
		inline void _initStackAllocatedString(POINTER<CHR> string);

		/* FUNCTION
		 * Copy the stack allocated string to the main data store.
		 */
		inline void _copyStackAllocatedStringToMain();

	private:
		POINTER<CHR> myStringData;	// main string container
		POINTER<CHR> myEndPtr;	// end pointer of the string
		POINTER<CHR> myBeginPtr;	// begin pointer of the string

		UI32 myStringSize = 0;	// size of the string
		UI32 myAllocatedSize = 0;	// size of the allocation
	};
}

#endif // !_DATA_TYPES_STRING_H

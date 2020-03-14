#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_POINTER_H
#define _DYNAMIK_CENTRAL_DATA_HUB_POINTER_H

#include "core/global/datatypes.h"

namespace Dynamik {
	/* TEMPLATED
	 * Pointer class for the Dynamik Engine.
	 * This class stores a pointer and supports address manipulation.
	 *
	 * @warn: TYPE should not be void (any half type). void* is accepted.
	 */
	template<class TYPE>
	class POINTER {
		/* DEFAULTS */
	public:
		/* CONSTRUCTOR
		 * Default constructor.
		 */
		POINTER() : myPointer(nullptr), myTypeSize(sizeof(TYPE)) {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param address: address of the data to be stored.
		 */
		POINTER(TYPE* address) : myPointer(address), myTypeSize(sizeof(TYPE)) {}

		/* DESTRUCTOR
		 * Default destructor.
		 */
		~POINTER() {}

		/* PUBLIC FUNCTIONS */
	public:
		/* FUNCTION
		 * Get the pointer.
		 */
		TYPE* get() const { return myPointer; }

		/* FUNCTION
		 * Get the pointer address.
		 */
		TYPE** getAddress() const { return &(TYPE*)myPointer; }

		/* FUNCTION
		 * Get the type size of the pointer.
		 */
		UI32 getTypeSize() const { return myTypeSize; }

		/* FUNCTION
		 * Set a value to the pointer.
		 */
		void set(const TYPE& value) { *myPointer = value; }

		/* FUNCTION
		 * Set a value to the pointer using an index.
		 *
		 * @param value: The value to be stored.
		 * @param index: Index to where to store the value.
		 * @warn: Does not validate the index before assigning.
		 */
		void setIndexed(const TYPE& value, const UI32 index) { myPointer[index] = value; }

		/* FUNCTION
		 * Check if the pointer is valid.
		 * Returns true if the pointer has data or not a null pointer.
		 */
		B1 isValid() const { return ((myPointer != nullptr) || (sizeof(myPointer) > sizeof(TYPE*))); }

		/* FUNCTION
		 * Assign the pointer to a gien pointer.
		 *
		 * @param pointer: The pointer to be assigned.
		 */
		void assign(VPTR pointer) { myPointer = (TYPE*)pointer; }

		/* FUNCTION
		 * Turn the pointer to a nullptr.
		 */
		void turnNull() { myPointer = nullptr; }

		/* FUNCTION
		 * Increment the address of the pointer by a value.
		 *
		 * @param value: Value to be incremented by.
		 */
		TYPE* increment(UI32 value)
		{
			return (this->myPointer = (TYPE*)(((UI64)this->myPointer) + (value * myTypeSize)));
		}

		/* FUNCTION
		 * Decrement the address of the pointer by a value.
		 *
		 * @param value: Value to be decremented by.
		 */
		TYPE* decrement(UI32 value)
		{
			return (this->myPointer = (TYPE*)(((UI64)this->myPointer) - (value * myTypeSize)));
		}

		/* PUBLIC OPERATORS */
	public:
		/* OPERATOR
		 * Return the de-referenced pointer.
		 */
		operator TYPE() { return *myPointer; }

		/* OPERATOR
		 * Return the pointer.
		 */
		operator TYPE* () { return myPointer; }

		/* OPERATOR
		 * Return the pointer.
		 */
		operator const TYPE* () { return myPointer; }

		/* OPERATOR
		 * Return the casted pointer.
		 */
		operator VPTR() { return myPointer; }

		/* OPERATOR
		 * Return the casted pointer.
		 */
		template<class SUB_TYPE>
		operator SUB_TYPE() { return (SUB_TYPE)myPointer; }

		/* OPERATOR
		 * Return the de-referenced pointer.
		 */
		TYPE operator*() { return *myPointer; }

		/* OPERATOR
		 * Return the de-referenced pointer.
		 */
		TYPE operator->() { return *myPointer; }

		/* OPERATOR
		 * Increment the address of the pointer by a value.
		 *
		 * @param increment: Value that is added to the pointer.
		 */
		TYPE* operator+(const UI32 increment) { return this->increment(increment); }

		/* OPERATOR
		 * Increment the address of the pointer by a value and assign it to this.
		 *
		 * @param increment: Value that is added to the pointer.
		 */
		TYPE* operator+=(const UI32 increment) { return this->increment(increment); }

		/* OPERATOR
		 * Decrement the address of the pointer by a value.
		 *
		 * @param decrement: Value to be subtracted to the pointer.
		 */
		TYPE* operator-(const UI32 decrement) { return this->decrement(decrement); }

		/* OPERATOR
		 * Decrement the address of the pointer by a value and assign to this.
		 *
		 * @param decrement: Value to be subtracted to the pointer.
		 */
		TYPE* operator-=(const UI32 decrement) { return this->decrement(decrement); }

		/* OPERATOR
		 * Increment the address of the pointer by one.
		 */
		TYPE* operator++(I32) { return this->increment(1); }

		/* OPERATOR
		 * Decrement the address of the pointer by one.
		 */
		TYPE* operator--(I32) { return this->decrement(1); }

		/* OPERATOR
		 * Check if the pointer is equal to this pointer.
		 *
		 * @param ptr: Pointer to be checked with.
		 */
		B1 operator==(const POINTER<TYPE>& ptr) { return ((UI64)this->myPointer) == ((UI64)ptr.myPointer); }

		/* OPERATOR
		 * Asign the address of one POINTER to this.
		 *
		 * @param ptr: Pointer to the address to be assigned.
		 */
		void operator()(const POINTER<TYPE>& ptr) { this->myPointer = ptr.get(); }

		/* OPERATOR
		 * Asign a value to the pointer.
		 *
		 * @param value: Value to be assigned to the pointer.
		 */
		void operator=(const TYPE& value) { *myPointer = value; }

		/* OPERATOR
		 * Asign a value to the pointer.
		 *
		 * @param ptr: Pointer to be copied to the pointer.
		 */
		void operator=(const POINTER<TYPE>& ptr) { this->myPointer = ptr.myPointer; }

		/* OPERATOR
		 * Access data at a given index in the pointer.
		 *
		 * @param index: Index of the data to be retrieved.
		 * @warn: Does not validate the index before retrieving.
		 */
		TYPE operator[](const UI32 index) const { return myPointer[index]; }

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		TYPE* myPointer = nullptr;	// pointer data store
		UI32 myTypeSize = 0;	// size of the type
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_POINTER_H

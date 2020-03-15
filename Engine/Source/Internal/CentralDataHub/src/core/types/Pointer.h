#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_POINTER_H
#define _DYNAMIK_CENTRAL_DATA_HUB_POINTER_H

#include "core/global/datatypes.h"
#include "core/global/typecheck.h"

#include <type_traits>

namespace Dynamik {
	/* TEMPLATED
	 * Pointer class for the Dynamik Engine.
	 * This class stores a pointer and supports address manipulation.
	 * POINTER basically acts as '*' and '&' in C++ with added functionalities.
	 *
	 * @warn: TYPE should not be void (any half type). void* is accepted.
	 */
	template<class TYPE>
	class POINTER {
		/* DEFAULTS */

		/* STATIC ASSERT
		 * Check if the Template argument is not void.
		 */
		//static_assert(Constant<B1, (typeid(TYPE) == typeid(void))>::isValue, "POINTER<TYPE> Does not support void!");
		DMK_STATIC_TYPE_CHECK(TYPE, void, "POINTER<TYPE> Does not support void!");

	public:
		/* CONSTRUCTOR
		 * Default constructor.
		 */
		POINTER() : myPointer(nullptr) {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param address: address of the data to be stored.
		 */
		POINTER(const TYPE* address) : myPointer((TYPE*)address) {}

		/* CONSTRUCTOR
		 * Default constructor.
		 *
		 * @param address: address of the data to be stored.
		 */
		POINTER(const VPTR address) : myPointer((TYPE*)address) {}

		/* DESTRUCTOR
		 * Default destructor.
		 */
		~POINTER() { myPointer = nullptr; }

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
		 * Get the pointer as a UI64 value.
		 * Converts the address sotred in the pointer to a unsigned 64bit integer.
		 */
		UI64 getPointerAsInteger() const { return (UI64)myPointer; }

		/* FUNCTION
		 * Get the pointer address as a UI64 value.
		 * Converts the pointer address to a unsigned 64bit integer.
		 */
		UI64 getAddressAsInteger() const { return (UI64)&myPointer; }

		/* FUNCTION
		 * Get the type size of the pointer.
		 */
		UI32 getTypeSize() const { return sizeof(TYPE); }

		/* FUNCTION
		 * Set a value to the pointer.
		 */
		void set(const TYPE value) { *myPointer = value; }

		/* FUNCTION
		 * Set a value to the pointer at a given index.
		 *
		 * @param index: Index to be updated.
		 * @param value: Value to be set.
		 * @warn: Index is not validated before or after assignment.
		 */
		void setIndexed(const UI32& index, const TYPE& value) { myPointer[index] = value; }

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
			return (this->myPointer = (TYPE*)(((UI64)this->myPointer) + (value * sizeof(TYPE))));
		}

		/* FUNCTION
		 * Decrement the address of the pointer by a value.
		 *
		 * @param value: Value to be decremented by.
		 */
		TYPE* decrement(UI32 value)
		{
			return (this->myPointer = (TYPE*)(((UI64)this->myPointer) - (value * sizeof(TYPE))));
		}

		/* FUNCTION
		 * Return the data stored at a given index in the pointer.
		 * Indexed value can be modified in the pointer.
		 *
		 * @param index: INdex to be accessed.
		 * @warn: Index is not validated before or after indexing.
		 */
		TYPE& at(const UI32& index) { return myPointer[index]; }

		/* FUNCTION
		 * Return the data stored at a given index in the pointer.
		 * Indexed value cannot be modified in the pointer.
		 *
		 * @param index: INdex to be accessed.
		 * @warn: Index is not validated before or after indexing.
		 */
		TYPE at(const UI32& index) const { return myPointer[index]; }

		/* PUBLIC OPERATORS */
	public:
		/* OPERATOR
		 * Return the de-referenced pointer.
		 */
		operator TYPE() { return *this->myPointer; }

		/* OPERATOR
		 * Return the pointer.
		 */
		operator TYPE* () { return this->myPointer; }

		/* OPERATOR
		 * Return the pointer.
		 */
		operator const TYPE* () { return this->myPointer; }

		/* OPERATOR
		 * Return the casted pointer.
		 */
		operator VPTR() { return this->myPointer; }

		/* OPERATOR
		 * Return the casted pointer.
		 */
		template<class SUB_TYPE>
		operator SUB_TYPE() { return (SUB_TYPE)this->myPointer; }

		/* OPERATOR
		 * Return the de-referenced pointer.
		 */
		TYPE operator*() { return *this->myPointer; }

		/* OPERATOR
		 * Return the de-referenced pointer.
		 */
		TYPE* operator->() { return this->myPointer; }

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
		 * Asign an address to the pointer.
		 * Accepts void pointers.
		 *
		 * @param value: Value to be assigned to the pointer.
		 */
		void operator=(const VPTR address) { this->myPointer = (TYPE*)address; }

		/* OPERATOR
		 * Asign a value to the pointer.
		 *
		 * @param value: Value to be assigned to the pointer.
		 */
		void operator=(const TYPE& value) { this->set(value); }

		/* OPERATOR
		 * Asign a value to the pointer.
		 *
		 * @param ptr: Pointer to be copied to the pointer.
		 */
		void operator=(const POINTER<TYPE>& ptr) { this->myPointer = ptr.myPointer; }

		/* OPERATOR
		 * Access data at a given index in the pointer.
		 * Indexed value can be modified in the pointer.
		 *
		 * @param index: Index of the data to be retrieved.
		 * @warn: Does not validate the index before retrieving.
		 */
		TYPE& operator[](const UI32& index) { return this->myPointer[index]; }

		/* OPERATOR
		 * Access data at a given index in the pointer.
		 * Indexed value cannot be modified in the pointer.
		 *
		 * @param index: Index of the data to be retrieved.
		 * @warn: Does not validate the index before retrieving.
		 */
		TYPE operator[](const UI32& index) const { return this->myPointer[index]; }

		/* PRIVATE VARIABLES AND CONSTANTS */
	private:
		TYPE* myPointer = nullptr;	// pointer data store
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_POINTER_H

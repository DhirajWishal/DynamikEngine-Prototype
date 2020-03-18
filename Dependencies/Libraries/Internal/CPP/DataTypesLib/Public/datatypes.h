#pragma once

#ifndef _DYNAMIK_DATA_TYPES_DATATYPES_H
#define _DYNAMIK_DATA_TYPES_DATATYPES_H

namespace Dynamik {
	/*
	* Convention:
	*	<Type><Size in Bytes>
	* Pointers:
	*	<Type>PTR
	*/

	typedef unsigned char UI8;			// unsigned 8 bit integer
	typedef unsigned short UI16;		// unsigned 16 bit integer
	typedef unsigned int UI32;			// unsigned 32 bit integer
	typedef unsigned long long UI64;	// unsigned 64 bit integer

	typedef signed char SI8;			// signed 8 bit integer
	typedef signed short SI16;			// signed 16 bit integer
	typedef signed int SI32;			// signed 32 bit integer
	typedef signed long long SI64;		// signed 64 bit integer

	typedef signed char I8;				// 8 bit integer
	typedef short I16;					// 16 bit integer
	typedef int I32;					// 32 bit integer
	typedef long long I64;				// 64 bit integer

	typedef bool B1;					// bool
	typedef float F32;					// float
	typedef double D64;					// double
	typedef long double LD64;			// long double

	typedef void* VPTR;					// void pointer
	typedef char* CPTR;					// char pointer
	typedef wchar_t* WCPTR;				// char pointer
	typedef const char* CCPTR;			// const char pointer
	typedef unsigned char* UCPTR;		// unsigned char pointer
	typedef const wchar_t* CWCPTR;		// const char pointer

	typedef char CHR;					// char (ASCII)
	typedef wchar_t WCHR;				// wide char (UTF-8)

	typedef UI8 BYTE;					// byte of data
	typedef BYTE* BPTR;					// byte pointer
}
#define DMK_UINT_8				::UI8
#define DMK_UINT_16				::UI16
#define DMK_UINT_32				::UI32
#define DMK_UINT_64				::UI64

#define DMK_SINT_8				::SI8
#define DMK_SINT_16				::SI16
#define DMK_SINT_32				::SI32
#define DMK_SINT_64				::SI64

#define DMK_INT_8				::I8
#define DMK_INT_16				::I16
#define DMK_INT_32				::I32
#define DMK_INT_64				::I64
#define DMK_INT					INT_32

#define DMK_BOOL				::B1
#define DMK_FLOAT_32			::F32
#define DMK_DOUBLE_64			::D64
#define DMK_FLOAT				FLOAT_32
#define DMK_DOUBLE				DOUBLE_64

#define DMK_VOID_PTR			::VPTR
#define DMK_CHAR_PTR			::CPTR
#define DMK_WCHAR_PTR			::WCPTR
#define DMK_CONST_CHAR_PTR		::CCPTR
#define DMK_UNSIGNED_CHAR_PTR	::UCPTR
#define DMK_CONST_WCHAR_PTR		::CWCPTR

#define DMK_BYTE				::BYTE
#define DMK_BYTE_PTR			::BPTR

#ifdef _UNICODE
#define DMK_TEXT(...)			__VA_ARGS__

#else
#define DMK_TEXT(...)			__VA_ARGS__

#endif
#define DMK_INTERNAL_TEXT(...)	__VA_ARGS__

#endif	// !_DATA_TYPES_DATATYPES_H

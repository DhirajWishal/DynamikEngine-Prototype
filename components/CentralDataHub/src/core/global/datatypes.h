#pragma once

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DATATYPES_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DATATYPES_H

#include <any>

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
	typedef const char* CCPTR;			// const char pointer
	typedef unsigned char* UCPTR;		// unsigned char pointer

	typedef char CHR;					// char (ASCII)
	typedef wchar_t WCHR;				// wide char (UTF-8)
	
	typedef std::any ANY;				// any data type

	typedef UI8 BYTE;					// byte of data
	typedef BYTE* BPTR;					// byte pointer
}

#define DMK_UINT_8				::Dynamik::UI8
#define DMK_UINT_16				::Dynamik::UI16
#define DMK_UINT_32				::Dynamik::UI32
#define DMK_UINT_64				::Dynamik::UI64

#define DMK_SINT_8				::Dynamik::SI8
#define DMK_SINT_16				::Dynamik::SI16
#define DMK_SINT_32				::Dynamik::SI32
#define DMK_SINT_64				::Dynamik::SI64

#define DMK_INT_8				::Dynamik::I8
#define DMK_INT_16				::Dynamik::I16
#define DMK_INT_32				::Dynamik::I32
#define DMK_INT_64				::Dynamik::I64
#define DMK_INT					DMK_INT_32

#define DMK_BOOL				::Dynamik::B1
#define DMK_FLOAT_32			::Dynamik::F32
#define DMK_DOUBLE_64			::Dynamik::D64
#define DMK_FLOAT				DMK_FLOAT_32
#define DMK_DOUBLE				DMK_DOUBLE_64

#define DMK_VOID_PTR			::Dynamik::VPTR
#define DMK_CHAR_PTR			::Dynamik::CPTR
#define DMK_CONST_CHAR_PTR		::Dynamik::CCPTR
#define DMK_UNSIGNED_CHAR_PTR	::Dynamik::UCPTR

#define DMK_BYTE				::Dynamik::BYTE
#define DMK_BYTE_PTR			::Dynamik::BPTR

#endif	// !_DYNAMIK_CENTRAL_DATA_HUB_DATATYPES_H

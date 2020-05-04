#pragma once

#ifndef _DYNAMIK_DATA_TYPES_DATATYPES_H
#define _DYNAMIK_DATA_TYPES_DATATYPES_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>



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

	typedef unsigned char UCHR;			// unsigned char

	/* Function to cast from one type to another */
	template<class RTYPE, class PTYPE>
	RTYPE Cast(const PTYPE& data)
	{
		return RTYPE(data);
	}

	/* GLM WRAPPERS */
	typedef glm::vec1 VEC1;
	typedef glm::vec2 VEC2;
	typedef glm::vec3 VEC3;
	typedef glm::vec4 VEC4;

	typedef glm::uvec1 UVEC1;
	typedef glm::uvec2 UVEC2;
	typedef glm::uvec3 UVEC3;
	typedef glm::uvec4 UVEC4;
	
	typedef glm::mat2 MAT2;
	typedef glm::mat3 MAT3;
	typedef glm::mat4 MAT4;
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
#define DMK_INT					INT_32

#define DMK_BOOL				::Dynamik::B1
#define DMK_FLOAT_32			::Dynamik::F32
#define DMK_DOUBLE_64			::Dynamik::D64
#define DMK_FLOAT				FLOAT_32
#define DMK_DOUBLE				DOUBLE_64

#define DMK_VOID_PTR			::Dynamik::VPTR
#define DMK_CHAR_PTR			::Dynamik::CPTR
#define DMK_WCHAR_PTR			::Dynamik::WCPTR
#define DMK_CONST_CHAR_PTR		::Dynamik::CCPTR
#define DMK_UNSIGNED_CHAR_PTR	::Dynamik::UCPTR
#define DMK_CONST_WCHAR_PTR		::Dynamik::CWCPTR

#define DMK_BYTE				::Dynamik::BYTE
#define DMK_BYTE_PTR			::Dynamik::BPTR

#ifdef _UNICODE
#define DMK_TEXT(...)			__VA_ARGS__

#else
#define DMK_TEXT(...)			__VA_ARGS__

#endif
#define DMK_INTERNAL_TEXT(...)	__VA_ARGS__

#endif	// !_DATA_TYPES_DATATYPES_H

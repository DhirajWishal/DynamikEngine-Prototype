#pragma once
#ifndef _DYNAMIK_DATA_TYPES_HASH_MAP_H
#define _DYNAMIK_DATA_TYPES_HASH_MAP_H

#include "datatypes.h"
#include "Pointer.h"

namespace Dynamik {
	template<class TYPE_KEY, class TYPE_DATA>
	class HashMap {
		using TK = TYPE_KEY;
		using TD = TYPE_DATA;

	public:
		HashMap() {}
		~HashMap() {}
	};

	UI32 _getCCPTRSize(CCPTR string)
	{
		UI32 _size = 0;
		while (string[_size] != '\0') _size++;
		return _size;
	}

	CCPTR getHash(CCPTR data)
	{
		UI32 _size = _getCCPTRSize(data);
		const UI32 _bufferSize = 16;
		CHR _buffer[_bufferSize] = { 0 };
		UI32 _oldValue = 0;
		UI32 _index = 0;
		UI32 _value = 0;

		if (_size < _bufferSize)
		{
			for (UI32 _itr = 0; _itr < _bufferSize; _itr++)
			{
				if (_index == _size)
					_index = 0;

				_value = (UI32)data[_index];
				_buffer[_itr] = (((_value ^ _oldValue) ^ _itr)) + 65;
				_oldValue = _value;

				_index++;
			}
		}
		else
		{
			for (UI32 _itr = 0; _itr < _size; _itr++)
			{
				if (_index == _bufferSize)
					_index = 0;

				_value = (UI32)data[_itr];
				_buffer[_index] = (((_value ^ _oldValue) ^ _itr) ^ _buffer[_index]) + 65;
				_oldValue = _value;

				_index++;
			}
		}

		for (UI32 _itr = 0; _itr < _bufferSize; _itr++)
			std::cout << _buffer[_itr];

		return _buffer;
	}
}

#endif // !_DYNAMIK_DATA_TYPES_HASH_MAP_H

#pragma once
#ifndef _DYNAMIK_DATA_TYPES_PAIR_H
#define _DYNAMIK_DATA_TYPES_PAIR_H

namespace Dynamik {
	template<class TYPE_1, class TYPE_2>
	class PAIR {
	public:
		TYPE_1 firstValue;
		TYPE_2 secondValue;

		PAIR() {}
		~PAIR() {}

		TYPE_1 getFirst() {
			return firstValue;
		}

		TYPE_2 getSecond() {
			return firstValue;
		}
	};
}

#endif // !_DATA_TYPES_PAIR_H

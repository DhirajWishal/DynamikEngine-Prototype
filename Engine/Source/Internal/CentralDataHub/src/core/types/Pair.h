#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_PAIR_H
#define _DYNAMIK_CENTRAL_DATA_HUB_PAIR_H

namespace Dynamik {
	template<class TYPE_1, class TYPE_2>
	class Pair {
	public:
		TYPE_1 first;
		TYPE_2 second;

		Pair() {}
		~Pair() {}

		TYPE_1 getFirst() {
			return first;
		}

		TYPE_2 getSecond() {
			return second;
		}
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_PAIR_H

#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_ITERATOR_H
#define _DYNAMIK_CENTRAL_DATA_HUB_ITERATOR_H

namespace Dynamik {
	template<class TYPE>
	class Iterator {
	public:
		Iterator() {}
		Iterator(TYPE* address) : data(address) {}
		~Iterator() {}

		Iterator next() { return ++Iterator.data; }

		TYPE* data = nullptr;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_ITERATOR_H

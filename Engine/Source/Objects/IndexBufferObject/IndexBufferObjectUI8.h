#pragma once
#ifndef _DYNAMIK_INDEX_BUFFER_OBJECT_UI8_H
#define _DYNAMIK_INDEX_BUFFER_OBJECT_UI8_H

#include "IndexBufferObject.h"
#include <vector>

namespace Dynamik {
	class IndexBufferObjectUI8 : public IndexBufferObject {
	public:
		IndexBufferObjectUI8() {}
		~IndexBufferObjectUI8() {}

		std::vector<UI8> object;
	};
}

#endif // !_DYNAMIK_INDEX_BUFFER_OBJECT_UI8_H

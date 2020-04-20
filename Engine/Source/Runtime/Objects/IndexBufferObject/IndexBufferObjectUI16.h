#pragma once
#ifndef _DYNAMIK_INDEX_BUFFER_OBJECT_UI16_H
#define _DYNAMIK_INDEX_BUFFER_OBJECT_UI16_H

#include "IndexBufferObject.h"
#include <Public/Array.h>

namespace Dynamik {
	class IndexBufferObjectUI16 : public IndexBufferObject {
	public:
		IndexBufferObjectUI16() {}
		~IndexBufferObjectUI16() {}

		ARRAY<UI16> object;
	};
}

#endif // !_DYNAMIK_INDEX_BUFFER_OBJECT_UI16_H

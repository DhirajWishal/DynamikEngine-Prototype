#pragma once
#ifndef _DYNAMIK_INDEX_BUFFER_OBJECT_UI32_H
#define _DYNAMIK_INDEX_BUFFER_OBJECT_UI32_H

#include "IndexBufferObject.h"
#include "DataTypesLib/Public/Array.h"

namespace Dynamik {
	class IndexBufferObjectUI32 : public IndexBufferObject {
	public:
		IndexBufferObjectUI32() {}
		~IndexBufferObjectUI32() {}

		ARRAY<UI32> object;
	};
}

#endif // !_DYNAMIK_INDEX_BUFFER_OBJECT_UI32_H

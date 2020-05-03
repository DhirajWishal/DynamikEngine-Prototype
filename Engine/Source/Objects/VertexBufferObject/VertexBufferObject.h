#pragma once
#ifndef _DYNAMIK_VERTEX_BUFFER_OBJECT_H
#define _DYNAMIK_VERTEX_BUFFER_OBJECT_H

#include "../DMKObject/DMKObject.h"
#include "../IndexBufferObject/IndexBufferObjectUI8.h"
#include "../IndexBufferObject/IndexBufferObjectUI16.h"
#include "../IndexBufferObject/IndexBufferObjectUI32.h"
#include <string>

namespace Dynamik {
	class VertexBufferObject : public DMKObject {
	public:
		VertexBufferObject() {}
		virtual ~VertexBufferObject() {}

		virtual void loadVertexData(std::string path) {}

		virtual IndexBufferObjectUI8 getIndexBufferObjectUI8() {}
		virtual IndexBufferObjectUI16 getIndexBufferObjectUI16() {}
		virtual IndexBufferObjectUI32 getIndexBufferObjectUI32() {}
	};
}

#endif // !_DYNAMIK_VERTEX_BUFFER_OBJECT_H

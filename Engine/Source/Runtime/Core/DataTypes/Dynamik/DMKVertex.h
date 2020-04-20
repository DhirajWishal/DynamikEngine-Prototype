#pragma once
#ifndef _DYNAMIK_DATA_TYPES_VERTEX_H
#define _DYNAMIK_DATA_TYPES_VERTEX_H

namespace Dynamik {
	class DMKVertex {
	public:
		DMKVertex() {}
		virtual ~DMKVertex() {}

		virtual bool operator==(const DMKVertex* other) { return false; }
	};
}

#endif // !_DYNAMIK_DATA_TYPES_VERTEX_H

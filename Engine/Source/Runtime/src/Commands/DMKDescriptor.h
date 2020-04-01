#pragma once
#ifndef _DYNAMIK_DESCRIPTOR_H
#define _DYNAMIK_DESCRIPTOR_H

#include "core/data store/DMKObject.h"

namespace Dynamik {
	enum class DMKDescriptorType {
		DMK_DESCRIPTOR_TYPE_CREATE_INSTANCE,
	};

	class DMKDescriptor : public DMKObject {
	public:
		DMKDescriptor(DMKDescriptorType type) : myType(type) {}
		~DMKDescriptor() {}

		DMKDescriptorType myType;
	};
}

#endif // !#define _DYNAMIK_DESCRIPTOR_H

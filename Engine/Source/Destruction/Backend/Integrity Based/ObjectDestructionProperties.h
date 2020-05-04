#pragma once
#ifndef _DYNAMIK_DESTRUCTION_ENGINE_OBJECT_DESTRUCTION_PROPERTIES_H
#define _DYNAMIK_DESTRUCTION_ENGINE_OBJECT_DESTRUCTION_PROPERTIES_H

#include "Public/datatypes.h"
#include "DestructionEntity.h"

namespace Dynamik {
	struct  DMKObjectDestructionProperties {
		F32 integrity = 1.0;
		F32 activeSurfaceArea = 1.0f;
		F32 mass = 1.0f;
		F32 velocity = 0.0f;
	};
}

#endif // !_DYNAMIK_DESTRUCTION_ENGINE_OBJECT_DESTRUCTION_PROPERTIES_H

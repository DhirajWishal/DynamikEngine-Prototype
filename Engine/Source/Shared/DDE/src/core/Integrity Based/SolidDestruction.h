#pragma once
#ifndef _DYNAMIK_DESTRUCTION_ENGINE_INTEGRITY_BASED_SOLID_DESTRUCTION_H
#define _DYNAMIK_DESTRUCTION_ENGINE_INTEGRITY_BASED_SOLID_DESTRUCTION_H

#include "DestructionEntity.h"

namespace Dynamik {
	class SolidDestruction : public DestructionEntity {
	public:
		SolidDestruction() {}
		SolidDestruction(DMKDestructionType type)
			: DestructionEntity(type, DMKDestructionBehaviour::DMK_DESTRUCTION_BEHAVIOUR_SOLID) {}
		virtual ~SolidDestruction() {}
	};
}

#endif // !_DYNAMIK_DESTRUCTION_ENGINE_INTEGRITY_BASED_SOLID_DESTRUCTION_H

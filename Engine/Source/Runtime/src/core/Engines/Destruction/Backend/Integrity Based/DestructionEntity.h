#pragma once
#ifndef _DYNAMIK_DESTRUCTION_ENGINE_ENTITY_H
#define _DYNAMIK_DESTRUCTION_ENGINE_ENTITY_H

namespace Dynamik {
	enum class DMKDestructionType {
		DMK_DESTRUCTION_TYPE_IMPACT,
		DMK_DESTRUCTION_TYPE_EXPLOSION,

		DMK_DESTRUCTION_TYPE_MAX_ENUM
	};

	enum class DMKDestructionBehaviour {
		DMK_DESTRUCTION_BEHAVIOUR_SOLID,
		DMK_DESTRUCTION_BEHAVIOUR_LIQUID,
		DMK_DESTRUCTION_BEHAVIOUR_FRAGILE,
		DMK_DESTRUCTION_BEHAVIOUR_METALIC,
		DMK_DESTRUCTION_BEHAVIOUR_PAPER,
		DMK_DESTRUCTION_BEHAVIOUR_WOOD,
		DMK_DESTRUCTION_BEHAVIOUR_RUBBER,

		DMK_DESTRUCTION_BEHAVIOUR_CUSTOM,

		DMK_DESTRUCTION_BEHAVIOUR_MAX_ENUM
	};

	/* BASE CLASS
	 * Destruction Entity
	 */
	class DestructionEntity {
	public:
		DestructionEntity() {}
		DestructionEntity(DMKDestructionType type, DMKDestructionBehaviour behaviour)
			: myDestructionType(type), myDestructionBehaviour(behaviour) {}
		virtual ~DestructionEntity() {}

		DMKDestructionType getType() { return myDestructionType; }
		DMKDestructionBehaviour getBehaviour() { return myDestructionBehaviour; }

	protected:
		DMKDestructionType myDestructionType = DMKDestructionType::DMK_DESTRUCTION_TYPE_IMPACT;
		DMKDestructionBehaviour myDestructionBehaviour = DMKDestructionBehaviour::DMK_DESTRUCTION_BEHAVIOUR_SOLID;
	};
}

#endif // !_DYNAMIK_DESTRUCTION_ENTITY_H

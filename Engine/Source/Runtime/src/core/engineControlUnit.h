#pragma once
#ifndef _DYNAMIK_ENGINE_CONTROL_UNIT_H
#define _DYNAMIK_ENGINE_CONTROL_UNIT_H

namespace Dynamik {
	class EngineControlUnit {
	public:
		EngineControlUnit() {}
		~EngineControlUnit() {}

		void initialize();
		void mainLoop();
		void shutdown();

	private:

	};
}

#endif // !_DYNAMIK_ENGINE_CONTROL_UNIT_H

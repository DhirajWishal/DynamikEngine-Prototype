#pragma once

#include "core.h"

namespace Dynamik {
	class DMK_API Application {
	public:
		Application();
		virtual ~Application();

		void run();

	private:
	};

	// Defined by the Client
	Application* createApplication();

}
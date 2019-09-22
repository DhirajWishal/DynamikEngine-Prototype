#pragma once

namespace Dynamik {
	namespace ADGR {

		class c_Renderer {
		public:
			c_Renderer() {}
			virtual ~c_Renderer() {}

			virtual void init() {}
			virtual void drawFrame() {}
			virtual void shutDown() {}
		};
	}
}

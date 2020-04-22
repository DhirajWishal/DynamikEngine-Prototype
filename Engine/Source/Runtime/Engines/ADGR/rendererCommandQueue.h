#pragma once
#ifndef _DMK_ADGR_RENDERER_COMMAND_QUEUE_H
#define _DMK_ADGR_RENDERER_COMMAND_QUEUE_H

#include <queue>

namespace Dynamik {
	namespace ADGR {
		enum class DMKRendererCommand {
			DMK_RENDERER_COMMAND_NONE,
			DMK_RENDERER_COMMAND_SUBMIT_RENDER_DATA,
		};

		struct DMKRendererCommandContainer {
			DMKRendererCommand command = DMKRendererCommand::DMK_RENDERER_COMMAND_NONE;

			VPTR data = nullptr;
		};

		class RendererCommandQueue {
		public:
			RendererCommandQueue() {}
			~RendererCommandQueue() {}

			void addCommand(DMKRendererCommandContainer& container);
			void addCommands(std::queue<DMKRendererCommandContainer> containers);
			void setCommands(std::queue<DMKRendererCommandContainer>& containers);

			DMKRendererCommandContainer getCommand();
			DMKRendererCommandContainer getCommandAndPop();
			std::queue<DMKRendererCommandContainer> getCommandQueue();

			void clear();

		private:
			std::queue<DMKRendererCommandContainer> myCommandQueue = {};
		};
	}
}

#endif // !_DMK_ADGR_RENDERER_COMMAND_QUEUE_H

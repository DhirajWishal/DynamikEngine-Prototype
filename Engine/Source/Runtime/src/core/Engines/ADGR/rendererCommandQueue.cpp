#include <dmkafx.h>
#include "rendererCommandQueue.h"

namespace Dynamik {
	namespace ADGR {
		void RendererCommandQueue::addCommand(DMKRendererCommandContainer& container)
		{
			myCommandQueue.push(container);

			std::string str = ("HELLO WORLD");
		}

		void RendererCommandQueue::addCommands(std::queue<DMKRendererCommandContainer> containers)
		{
			for (UI32 _itr = 0; _itr < containers.size(); _itr++) {
				myCommandQueue.push(containers.front());
				containers.pop();
			}
		}

		void RendererCommandQueue::setCommands(std::queue<DMKRendererCommandContainer>& containers)
		{
			myCommandQueue = containers;
		}

		DMKRendererCommandContainer RendererCommandQueue::getCommand()
		{
			return myCommandQueue.front();
		}

		DMKRendererCommandContainer RendererCommandQueue::getCommandAndPop()
		{
			auto command = myCommandQueue.front();
			myCommandQueue.pop();
			return command;
		}

		std::queue<DMKRendererCommandContainer> RendererCommandQueue::getCommandQueue()
		{
			auto _command = myCommandQueue;
			myCommandQueue = {};
			return _command;
		}

		void RendererCommandQueue::clear()
		{
			myCommandQueue = {};
		}
	}
}
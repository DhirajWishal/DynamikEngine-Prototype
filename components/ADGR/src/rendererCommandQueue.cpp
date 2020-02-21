#include <adgrafx.h>
#include "rendererCommandQueue.h"

namespace Dynamik {
	namespace ADGR {
		void RendererCommandQueue::addCommand(DMKRendererCommandContainer& container)
		{
			myCommandQueue.push(container);
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
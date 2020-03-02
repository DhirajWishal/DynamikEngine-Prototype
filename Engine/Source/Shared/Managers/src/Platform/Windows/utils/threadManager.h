#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_MANAGER_H
#define _DYNAMIK_MANAGERS_THREAD_MANAGER_H

namespace Dynamik {
	struct DMK_API ThreadDataContainer {
		VPTR myRawData = nullptr;
	};

	class DMK_API Thread {
	public:
		Thread() {}
		virtual ~Thread() {}

		virtual void init() {}
		virtual void mainLoop() {}
		virtual B1 loopEndCommand() { return terminate; }
		virtual void shutdown() {}

		void setIndex(UI32 index) { myThreadIndex = index; }
		UI32 getIndex() { return myThreadIndex; }
		UI32 getID() { return myThreadID; }
		void setDataContainer(ThreadDataContainer* container) { myDataContainer = container; }

	protected:
		UI32 myThreadID = 0;
		UI32 myThreadIndex = 0;
		B1 terminate = false;

		ThreadDataContainer* myDataContainer = nullptr;
	};

	class DMK_API ThreadManager {
	public:
		ThreadManager() {}
		~ThreadManager() {}

		void add(Thread* myThread);
		void run(UI32 index = 0);

	private:
		void runThread(Thread* thread);

		std::vector<Thread*> myThreadContainer;

	};
}

#endif // !_DYNAMIK_MANAGERS_THREAD_MANAGER_H

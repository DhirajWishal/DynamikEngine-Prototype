#pragma once
#ifndef _DYNAMIK_MANAGERS_THREAD_MANAGER_H
#define _DYNAMIK_MANAGERS_THREAD_MANAGER_H

namespace Dynamik {
	class ThreadManager {
	public:
		ThreadManager() {}
		~ThreadManager() {}

		template<class FUNC, class... ARGS>
		void add(FUNC func, ARGS&&... args);

	private:
	};
	template<class FUNC, class ...ARGS>
	inline void ThreadManager::add(FUNC func, ARGS&& ...args)
	{
		func(args);
	}
}

#endif // !_DYNAMIK_MANAGERS_THREAD_MANAGER_H

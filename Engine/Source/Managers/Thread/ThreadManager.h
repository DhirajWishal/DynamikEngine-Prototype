#pragma once
#ifndef _DYNAMIK_THREAD_MANAGER_H
#define _DYNAMIK_THREAD_MANAGER_H

#include "Objects/InternalFormat/InternalFormat.h"
#include "../Window/Window.h"

#include <vector>

namespace Dynamik {
	/* Thread manager for the Dynamik Engine.
	 * The engine creates a set of threads upon its creation. These are,
	 * - Rendering thread.
	 * - Event management/ Window manager thread.
	 * - Resource thread.
	 */
	class ThreadManager {
	public:
		ThreadManager() {}
		~ThreadManager() {}

		void initiateThreads(std::vector<POINTER<InternalFormat>> formats, std::vector<POINTER<DMKWindow>> handles);
		void updateInFlightObjects(std::vector<POINTER<InternalFormat>> formats);
		void updateWindowHandles(std::vector<POINTER<DMKWindow>> handles);

	public:
		static void _eventThread(POINTER<std::vector<POINTER<InternalFormat>>> formats, POINTER<std::vector<POINTER<DMKWindow>>> windowHandles);

	private:
		std::vector<POINTER<InternalFormat>> inFlightObjects;
		std::vector<POINTER<DMKWindow>> windowHandles;

		POINTER<std::thread> _eventThreadHandle;
	};
}

#endif // !_DYNAMIK_THREAD_MANAGER_H

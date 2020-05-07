#pragma once
#ifndef _DYNAMIK_THREAD_MANAGER_H
#define _DYNAMIK_THREAD_MANAGER_H

#include "Objects/InternalFormat/InternalFormat.h"
#include "../Window/Window.h"

#include "Public/Array.h"

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

		void initiateThreads(ARRAY<POINTER<InternalFormat>> formats, ARRAY<POINTER<DMKWindow>> handles);
		void updateInFlightObjects(ARRAY<POINTER<InternalFormat>> formats);
		void updateWindowHandles(ARRAY<POINTER<DMKWindow>> handles);

	public:
		static void _eventThread(POINTER<ARRAY<POINTER<InternalFormat>>> formats, POINTER<ARRAY<POINTER<DMKWindow>>> windowHandles);

	private:
		ARRAY<POINTER<InternalFormat>> inFlightObjects;
		ARRAY<POINTER<DMKWindow>> windowHandles;

		POINTER<std::thread> _eventThreadHandle;
	};
}

#endif // !_DYNAMIK_THREAD_MANAGER_H

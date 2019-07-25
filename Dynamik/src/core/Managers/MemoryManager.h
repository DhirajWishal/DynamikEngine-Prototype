#pragma once

/*
 Heap allocation manager for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		--/--/----
 IDE:		MS Visual Studio Community 2019

 MemoryManager.h file.
*/

#include "core/utils/DMK_DataTypes.h"

namespace Dynamik {
	class MemoryManager {
	public:
		typedef uint32 Pointer;

		MemoryManager();
		~MemoryManager();

		void allocate(uint32 size);
		Pointer getPointer();

	private:

	};

}

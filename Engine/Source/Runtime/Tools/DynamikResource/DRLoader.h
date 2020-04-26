#pragma once
#ifndef _DYNAMIK_RESOURCE_LOADER_H
#define _DYNAMIK_RESOURCE_LOADER_H

#include "DResFile.h"

namespace Dynamik {
	class DMKResLoader : public DMKObject {
	public:
		DMKResLoader() {}
		~DMKResLoader() {}

		DResFile loadFile(CCPTR path);
	};
}

#endif // !_DYNAMIK_RESOURCE_LOADER_H

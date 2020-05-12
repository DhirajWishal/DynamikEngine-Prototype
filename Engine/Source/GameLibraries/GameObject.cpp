#include "dmkafx.h"
#include "GameObject.h"

namespace Dynamik {
	DMKGameObject::~DMKGameObject()
	{
		uniformBufferDataStore.terminate();
	}
}
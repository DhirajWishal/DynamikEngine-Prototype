#pragma once
#ifndef _APPLICATION_VIKING_ROOM_H
#define _APPLICATION_VIKING_ROOM_H

#include "StaticObject.h"
using namespace Dynamik;

class VikingRoom : public DMKStaticObject {
public:
	VikingRoom() {}
	~VikingRoom() {}

	void initialize() override;
	DMKUniformBufferData onUpdate(DMKCameraData data) override;
};

#endif // !_APPLICATION_VIKING_ROOM_H

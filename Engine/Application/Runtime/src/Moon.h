#pragma once
#ifndef _APPLICATION_MOON_H
#define _APPLICATION_MOON_H

#include "StaticObject.h"
using namespace Dynamik;

class Moon : public Dynamik::DMKStaticObject {
public:
	Moon() {}
	~Moon() {}

	void initialize() override;
	DMKUniformBufferData onUpdate(DMKCameraData data) override;
};

#endif // !_APPLICATION_MOON_H

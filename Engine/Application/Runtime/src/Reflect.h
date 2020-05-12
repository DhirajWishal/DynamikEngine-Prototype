#pragma once
#ifndef _APPLICATION_REFLECT_OBJECT_H
#define _APPLICATION_REFLECT_OBJECT_H

#include "StaticObject.h"

class Reflect : public Dynamik::DMKStaticObject {
public:
	Reflect();
	~Reflect() {}

	void initialize() override;

	Dynamik::DMKUniformBufferData onUpdate(Dynamik::DMKCameraData camData) override final;
};

#endif // !_APPLICATION_REFLECT_OBJECT_H

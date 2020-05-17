#pragma once
#ifndef _APPLICATION_GOBLIN_H
#define _APPLICATION_GOBLIN_H

#include "AnimatedObject.h"
using namespace Dynamik;

class Goblin : public DMKAnimatedObject<100> {
public:
	Goblin() {}
	~Goblin() {}

	void initialize() override;
	DMKUniformBufferData onUpdate(DMKCameraData data, std::vector<MAT4> boneTransforms) override;
};

#endif // !_APPLICATION_GOBLIN_H

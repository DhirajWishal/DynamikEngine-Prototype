#pragma once
#ifndef _APPLICATION_SKYBOX_DEFINITION_H
#define _APPLICATION_SKYBOX_DEFINITION_H

#include "Dynamik.h"

class OceanSkybox : public Dynamik::DMKSkyboxObject {
public:
	OceanSkybox() {}
	~OceanSkybox() {}

	void initialize() override;

	DMKUniformBufferData onUpdate(DMKCameraData data) override;
};

#endif // !_APPLICATION_SKYBOX_DEFINITION_H

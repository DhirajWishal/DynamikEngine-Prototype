#pragma once
#ifndef _APPLICATION_SKYBOX_DEFINITION_H
#define _APPLICATION_SKYBOX_DEFINITION_H

#include "Dynamik.h"

class OceanSkybox : public Dynamik::DMKSkyboxObject {
public:
	OceanSkybox() 
	{
		type = Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX;
	}
	~OceanSkybox() {}
};

#endif // !_APPLICATION_SKYBOX_DEFINITION_H

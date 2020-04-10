#pragma once
#ifndef _SKYBOX_DEFINITION_H
#define _SKYBOX_DEFINITION_H

#include "Dynamik.h"
using namespace Dynamik;

class Skybox : public DMKSkyboxObject {
public:
	Skybox() 
	{
		myProperties.name = "SkyBox";
		myProperties.ID = "001";
		myProperties.location = "E:/Projects/Dynamik Engine/Game Repository/assets/assets/Skybox";
		myProperties.transformProperties.location = { 0.0f, 0.0f, 0.0f };
	}
	~Skybox() {}
};

#endif // !_SKYBOX_DEFINITION_H

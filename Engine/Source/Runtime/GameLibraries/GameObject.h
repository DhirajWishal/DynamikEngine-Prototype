#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_H
#define _DYNAMIK_GAME_OBJECT_H

/*
 GameObject data type for Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		19/06/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 GameObject.h file.
*/

/* MAIN INCLUDES */
#include "KeyBingings.h"
#include "core/log.h"
#include "Core/DataTypes/Dynamik/objectSpecifications.h"
#include "Objects/DMKObject/DMKObject.h"

#include "GameObjectDescriptors.h"

// Dynamik namespace
namespace Dynamik {
    class DMKGameObject : public DMKObject {
    public:
        DMKGameObject() {}
        virtual ~DMKGameObject() {}

        DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;

        DMKGameObjectDescriptor descriptor;
    };
}

#endif // !_DYNAMIK_GAME_OBJECT_H

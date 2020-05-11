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

#include "Events/KeyEventComponent.h"
#include "Events/MouseButtonEventComponent.h"
#include "Events/MouseScrollEventComponent.h"

#include "GameObjectDescriptors.h"

// Dynamik namespace
namespace Dynamik {
    /* Dynamik Game Object
     * This is the base class for all the objects given to the Dynamik Engine.
     * The user must derive a class from this object inorder to create an object which is compatible with the 
       Dynamik Engine.
     *
     */
    class DMKGameObject : public DMKObject {
    public:
        DMKGameObject() {}
        virtual ~DMKGameObject() {}

        /* VIRTUAL FUNCTION
         * This function returns the Uniform buffer data which is used to feed the shaders with the relavent
           Uniform data.
         * The data returned from this function must match the Uniform Buffer Descriptor or else an error will
           be flagged.
         */
        virtual DMKUniformBufferData onUpdate(DMKCameraData data) { return DMKUniformBufferData(); }

        /* VIRTUAL FUNCTION
         * Event handlers for the Dynamik Game Object.
         * Dynamik engine uses a threaded blocking event management system. When an event occurs, a 
           key event component structure is created. A new thread is then created to handle the event.
         */
        virtual void onKeyEvent(DMKKeyEventComponent component) {}
        virtual void onMouseButtonEvent(DMKMouseButtonEventComponent component) {}
        virtual void onMouseScrollEvent(DMKMouseScrollEventComponent component) {}

        DMKObjectType type = DMKObjectType::DMK_OBJECT_TYPE_STATIC;

        DMKGameObjectDescriptor descriptor;

    protected:
        DMKUniformBufferData uniformBufferDataStore;
    };
}

#endif // !_DYNAMIK_GAME_OBJECT_H

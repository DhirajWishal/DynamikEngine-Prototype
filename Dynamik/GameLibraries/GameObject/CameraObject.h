#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_CAMERA_H
#define _DYNAMIK_GAME_OBJECT_CAMERA_H

#include "GameObject.h"

namespace Dynamik {
	// Camera class
	class Camera : public GameObject {
	public:
		Camera(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Camera() {}

		/* ---------- ########## \\\\\\\\\\ CAMERA ////////// ########## ---------- */
		virtual void initCamera() {}
		virtual void initCamera(CameraProperties props) {}
	};
}

#endif

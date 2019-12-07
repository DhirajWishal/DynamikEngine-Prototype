#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_VIDEO_H
#define _DYNAMIK_GAME_OBJECT_VIDEO_H

#include "GameObject.h"

namespace Dynamik {
	// Video class
	class Video : public GameObject {
	public:
		Video(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Video() {}

		/* ---------- ########## \\\\\\\\\\ VIDEO ////////// ########## ---------- */
		virtual void initVideo() {}
	};
}

#endif

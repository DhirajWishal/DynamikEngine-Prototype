#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_AUDIO_H
#define _DYNAMIK_GAME_OBJECT_AUDIO_H

#include "GameObject.h"

namespace Dynamik {
	// Audio class
	class Audio : public GameObject {
	public:
		Audio(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Audio() {}

		/* ---------- ########## \\\\\\\\\\ AUDIO ////////// ########## ---------- */
		virtual void initAudio() {}
		virtual void initAudio(AudioProperties props) {}
	};
}

#endif

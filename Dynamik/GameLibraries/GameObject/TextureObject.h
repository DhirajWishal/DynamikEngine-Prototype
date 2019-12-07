#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_TEXTURE_H
#define _DYNAMIK_GAME_OBJECT_TEXTURE_H

#include "GameObject.h"

namespace Dynamik {
	// Texture class
	class Texture : public GameObject {
	public:
		Texture(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Texture() {}

		/* ---------- ########## \\\\\\\\\\ BIND THE TEXTURES ////////// ########## ---------- */
		virtual void loadTexture() {}
		virtual void deleteTexture() {}
	};

	class SkyBox : public Texture {
	public:
		SkyBox(GameObjectProperties& props) : Texture(props) {}
		virtual ~SkyBox() {}
	};

	class Sprites : public Texture {
	public:
		Sprites(GameObjectProperties& props) : Texture(props) {}
		virtual ~Sprites() {}
	};
}

#endif

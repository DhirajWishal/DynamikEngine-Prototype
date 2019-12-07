#pragma once

#ifndef _DYNAMIK_GAME_OBJECT_MESH_H
#define _DYNAMIK_GAME_OBJECT_MESH_H

#include "GameObject.h"
#include "TextureObject.h"

namespace Dynamik {
	// Mesh class
	class Mesh : public GameObject {
	public:
		Mesh(GameObjectProperties& props) : GameObject(props) {}
		virtual ~Mesh() {}

		/* ---------- ########## \\\\\\\\\\ TRANSFORM ////////// ########## ---------- */
		virtual void initTransform() {}
		virtual void setTransform(TransformProperties& properties) {}
		virtual void setTransformPosition(float* x, float* y, float* z) {}
		virtual void setTransformRotation(float* x, float* y, float* z) {}
		virtual void setTransformScale(float* x, float* y, float* z) {}
	};

	class Model : public Mesh, public Texture {
	public:
		Model(GameObjectProperties& props) : Mesh(props), Texture(props) {}
		virtual ~Model() {}
	};
}

#endif

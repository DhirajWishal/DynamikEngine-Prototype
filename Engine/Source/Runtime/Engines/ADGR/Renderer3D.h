#pragma once
#ifndef _DYNAMIK_ADGR_RENDERER_3D_H
#define _DYNAMIK_ADGR_RENDERER_3D_H

#include "Backend/base/RendererBackendBase.h"

namespace Dynamik {
	using namespace ADGR;

	enum class DMKRendererBackendLayer {
		DMK_RBL_VULKAN,
		DMK_RBL_OPENGL,
		DMK_RBL_DIRECTX_12,
	};

	class Renderer3D {
		Renderer3D() {}
		static Renderer3D myInstance;

	public:
		Renderer3D(const Renderer3D&) = delete;
		Renderer3D(Renderer3D&&) = delete;
		~Renderer3D() {}

		static Renderer3D& getInstance();

		static void setRBL(DMKRendererBackendLayer rbl, DMKRendererSettings settings);
		static void initializeStageOneRBL();
		static void initializeStageTwoRBL();
		static void initializeStageThreeRBL();

		static void addObject(GameObject* object);

	};
}

#endif // !_DYNAMIK_ADGR_RENDERER_3D_H

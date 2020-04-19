#include "dmkafx.h"
#include "Renderer3D.h"

#include "Backend/Vulkan/vulkanRenderer.h"

namespace Dynamik {
	using namespace ADGR;

	static vulkanRenderer myRenderer;

	Renderer3D Renderer3D::myInstance;

	Renderer3D& Renderer3D::getInstance()
	{
		return myInstance;
	}

	void Renderer3D::setRBL(DMKRendererBackendLayer rbl, DMKRendererSettings settings)
	{
		myRenderer = vulkanRenderer(settings);

		switch (rbl)
		{
		case Dynamik::DMKRendererBackendLayer::DMK_RBL_VULKAN:
			break;
		case Dynamik::DMKRendererBackendLayer::DMK_RBL_OPENGL:
			break;
		case Dynamik::DMKRendererBackendLayer::DMK_RBL_DIRECTX_12:
			break;
		default:
			break;
		}
	}

	void Renderer3D::initializeStageOneRBL()
	{
		myRenderer.initStageOne();
	}

	void Renderer3D::initializeStageTwoRBL()
	{
		myRenderer.initStageTwo();
	}

	void Renderer3D::initializeStageThreeRBL()
	{
		myRenderer.initStageThree();
	}
	
	void Renderer3D::addObject(GameObject* object)
	{
	}
}
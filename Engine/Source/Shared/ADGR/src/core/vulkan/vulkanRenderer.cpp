/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.cpp file
*/

#include "adgrafx.h"
#include "vulkanRenderer.h"
#include "Platform/windows.h"
#include "keyCodes.h"

#include "debugger.h"
#include "CentralDataHub.h"
#include "Platform.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		using namespace core;

#ifdef DMK_DEBUG
		Debugger::benchmark::FPS myFPSCal;

		// ----------
#endif

		// initialize the renderer
		void vulkanRenderer::init() {	// 14 inc
			initStageOne();
			initStageTwo();
			initStageThree();
		}

		// Basic one-time initializations
		void vulkanRenderer::initStageOne()
		{
			// initialize the window
			DMKWindowManagerInitInfo windowInitInfo;
			windowInitInfo.iconPaths = "E:/Projects/Dynamik Engine/Versions/Dynamik (Prototype)/Dependencies/Assets/icons/Dynamik.jpg";
			myWindowManager.initialize(windowInitInfo);
			my3DRenderer.setWindowHandle(myWindowManager.getHandle());

			my3DRenderer.initStageOne();
		}

		// object loading and initialization
		void vulkanRenderer::initStageTwo() {
			my3DRenderer.initStageTwo();
		}

		// final initialization
		void vulkanRenderer::initStageThree() {
			my3DRenderer.initStageThree();
		}

		// shutdown the renderer
		void vulkanRenderer::shutDown() {
			shutDownStageOne();
			shutDownStageTwo();
			shutDownStageThree();
		}

		// basic one-time shut down functions
		void vulkanRenderer::shutDownStageOne() {
			my3DRenderer.shutDownStageOne();
		}

		// per object shut down functions
		void vulkanRenderer::shutDownStageTwo() {
			my3DRenderer.shutDownStageTwo();
		}

		// final shut down functions
		void vulkanRenderer::shutDownStageThree() {
			my3DRenderer.shutDownStageThree();

			myWindowManager.terminate();
		}

		// draw frame
		void vulkanRenderer::drawFrame() {
#ifdef DMK_DEBUG
			myFPSCal.getFPS();	// FPS calculator

 // ----------
#endif
			myWindowManager.pollEvents();
			eventContainer = myWindowManager.getEventContainer();

			my3DRenderer.drawFrame(eventContainer);

			if (myWindowManager.isFrameBufferResized())
			{
				myWindowManager.frameBufferResizedUpdate(false);
				my3DRenderer.setWindowHandle(myWindowManager.getHandle());
				recreateSwapChain();
			}
		}

		// recreate the swapchain
		void vulkanRenderer::recreateSwapChain() {
			my3DRenderer.recreateSwapChain();
		}

		// events
		std::deque<DMKEventContainer>* vulkanRenderer::events() {
			return &eventContainer;
		}

		void vulkanRenderer::setFormats(ARRAY<RendererFormat>& rendererFormats) {
			ARRAY<ADGRVulkan3DObjectData> _objectDatas;

			for (UI32 _itr = 0; _itr < rendererFormats.size(); _itr++)
			{
				ADGRVulkan3DObjectData _object;
				_object.vertexShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath;
				_object.tessellationShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath;
				_object.geometryShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath;
				_object.fragmentShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath;

				_object.texturePaths = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.texturePaths;

				_object.vertexBufferObjects = &rendererFormats[_itr].myInternalFormat->myVertexBufferObjects;
				_object.indexBufferObjects = &rendererFormats[_itr].myInternalFormat->myIndexBufferObjects;

				_objectDatas.pushBack(_object);
			}

			my3DRenderer.getObjects(_objectDatas);
		}

		void vulkanRenderer::updateFormats(ARRAY<RendererFormat>& rendererFormats) {
			ARRAY<ADGRVulkan3DObjectData> _objectDatas;

			for (UI32 _itr = 0; _itr < rendererFormats.size(); _itr++)
			{
				ADGRVulkan3DObjectData _object;
				_object.vertexShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.vertexShaderPath;
				_object.tessellationShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.tessellationShaderPath;
				_object.geometryShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.geometryShaderPath;
				_object.fragmentShaderPath = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.renderableObjectProperties.fragmentShaderPath;

				_object.texturePaths = rendererFormats[_itr].myInternalFormat->myGameObject->myProperties.texturePaths;

				_object.vertexBufferObjects = &rendererFormats[_itr].myInternalFormat->myVertexBufferObjects;
				_object.indexBufferObjects = &rendererFormats[_itr].myInternalFormat->myIndexBufferObjects;

				_objectDatas.pushBack(_object);
			}

			my3DRenderer.getObjects(_objectDatas);
		}
	}
}
#endif
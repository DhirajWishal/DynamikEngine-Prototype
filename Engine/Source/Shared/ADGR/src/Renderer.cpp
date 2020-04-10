/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "Renderer.h"
#include "defines.h"

#include "debugger.h"

#ifdef DMK_PLATFORM_WINDOWS
#include "Platform/windows.h"

#else

#endif

#if defined(DMK_USE_VULKAN)
#include "core/vulkan/vulkanRenderer.h"

#elif defined (DMK_USE_DIRECTX)
#include "directx12/directx12Renderer.h"

#elif defined (DMK_USE_OPENGL)
#include "opengl/openglRenderer.h"

#else
#error Renderer Backend Undefined!

#endif

namespace Dynamik {
	namespace ADGR {
#if defined(DMK_USE_VULKAN)
		vulkanRenderer rendererCore;

#elif defined (DMK_USE_DIRECTX)
		directx12Renderer rendererCore;

#elif defined (DMK_USE_OPENGL)
		openglRenderer rendererCore;

#endif

		Renderer::Renderer() {
		}

		Renderer::~Renderer() {
		}

		void Renderer::setProgress(UI32* progress) {
			rendererCore.setProgress(progress);
		}

		void Renderer::initRenderer() {
			rendererCore.initStageOne();
			rendererCore.initStageTwo();
			rendererCore.initStageThree();
		}

		void Renderer::setRendererFormats(ARRAY<InternalFormat*>& internalFormats) {
			loadData(internalFormats, &myRendererFormats);
			rendererCore.setFormats(myRendererFormats);
		}

		void Renderer::draw() {
			rendererCore.drawFrame();
		}

		void Renderer::setVertices(ARRAY<Vertex>* vertices) {
			vertices;
		}

		void Renderer::addCommand(RendererCommandQueue commandQueue)
		{
			myCommandQueue = commandQueue;
		}

		void Renderer::run() {
			draw();
		}

		void Renderer::loadDataToUpdate(ARRAY<InternalFormat*>& internalFormats) {
			loadData(internalFormats, &myTemporaryFormats);
		}

		void Renderer::updateRendererFormats() {
			myRendererFormats = myTemporaryFormats;
			rendererCore.updateFormats3D(myRendererFormats);
			myTemporaryFormats.clear();
		}

		void Renderer::loadData(ARRAY<InternalFormat*>& internalFormats, ARRAY<RendererFormat>* formats) {
			for (auto format : internalFormats)
				formats->push_back(RendererFormat(format));

			{
				ARRAY<std::future<void>, DMKArrayDestructorCallMode::DMK_ARRAY_DESTRUCTOR_CALL_MODE_DESTRUCT_ALL> threads = {};
				for (I32 i = 0; i < internalFormats.size(); i++) {
					B1 isInitiated = false;
					RendererFormat* _localFormat = &formats->at(i);
					if (_localFormat->myInternalFormat->myGameObject->myProperties.type == DMKObjectType::DMK_OBJECT_TYPE_ANIMATION)
						continue;

					if (_localFormat->myInternalFormat->myVertexBufferObjects.size())
						for (UI32 itr = 0; itr < _localFormat->myInternalFormat->myVertexBufferObjects.size(); itr++)
							if (_localFormat->myInternalFormat->myVertexBufferObjects[itr].size())
								isInitiated = true;
					if (isInitiated || _localFormat->myInternalFormat->myGameObject->myProperties.objectPath.size() == 0)
						continue;

					_localFormat->myInternalFormat->myVertexBufferObjects.resize(1);
					_localFormat->myInternalFormat->myIndexBufferObjects.resize(1);

					DMKModelLoadInfo loadInfo = {};
					loadInfo.path = _localFormat->myInternalFormat->myGameObject->myProperties.objectPath[0];
					loadInfo.vertices = &_localFormat->myInternalFormat->myVertexBufferObjects[0];
					loadInfo.indices = &_localFormat->myInternalFormat->myIndexBufferObjects[0];
					loadInfo.vertexOffset = _localFormat->myInternalFormat->myGameObject->myProperties.transformProperties.location;
					threads.push_back(std::async(std::launch::async, loadModel, loadInfo));
				}
			}

			for (auto format : *formats) {
				if (format.myInternalFormat->myGameObject->myProperties.objectPath.size() == 0)
					continue;

				format.myInternalFormat->myVertexCounts.resize(1);
				format.myInternalFormat->myVertexCounts[0] = format.myInternalFormat->myVertexBufferObjects[0].size();
				format.myInternalFormat->myIndexCounts.resize(1);
				format.myInternalFormat->myIndexCounts[0] = format.myInternalFormat->myIndexBufferObjects[0].size();
			}
		}

		void Renderer::end() {
			rendererCore.shutDown();
		}

		B1 Renderer::getWindowCloseEvent() {
			return rendererCore.closeEvent();
		}

		void Renderer::idleCall() {
		}

		void Renderer::bindKeys() {
		}

		std::deque<DMKEventContainer>* Renderer::getEvents() {
			return rendererCore.events();
		}
	}
}
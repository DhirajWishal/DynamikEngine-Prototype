/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "adgrafx.h"
#include "Renderer.h"
#include "backend/defines.h"

#include "debugger.h"

#ifdef DMK_PLATFORM_WINDOWS
#include "Platform/windows.h"

#else

#endif

#if defined(DMK_USE_VULKAN)
#include "backend/vulkan/vulkanRenderer.h"

#elif defined (DMK_USE_DIRECTX)
#include "backend/directx12/directx12Renderer.h"

#elif defined (DMK_USE_OPENGL)
#include "backend/opengl/openglRenderer.h"

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

		void Renderer::setProgress(uint32_t* progress) {
			rendererCore.setProgress(progress);
		}

		void Renderer::initRenderer() {
			rendererCore.initStageOne();
			rendererCore.initStageTwo();
			rendererCore.initStageThree();
		}

		void Renderer::setRendererFormats(std::vector<InternalFormat*>& internalFormats) {
			loadData(internalFormats, &myRendererFormats);
			rendererCore.setFormats(myRendererFormats);
		}

		void Renderer::draw() {
			rendererCore.drawFrame();
		}

		void Renderer::setVertices(std::vector<Vertex>* vertices) {
			vertices;
		}

		void Renderer::run() {
			draw();
		}

		void Renderer::loadDataToUpdate(std::vector<InternalFormat*>& internalFormats) {
			loadData(internalFormats, &myTemporaryFormats);
		}

		void Renderer::updateRendererFormats() {
			myRendererFormats = myTemporaryFormats;
			rendererCore.updateFormats(myRendererFormats);
			myTemporaryFormats.clear();
		}

		void Renderer::loadData(std::vector<InternalFormat*>& internalFormats, std::vector<RendererFormat>* formats) {
			for (auto format : internalFormats)
				formats->push_back(RendererFormat(format));

			{
				std::vector<std::future<void>> threads = {};
				for (int i = 0; i < internalFormats.size(); i++) {
					bool isInitiated = false;
					RendererFormat* _localFormat = &formats->at(i);
					if (_localFormat->myInternalFormat->myVertexBufferObjects.size())
						for (uint32_t itr = 0; itr < _localFormat->myInternalFormat->myVertexBufferObjects.size(); itr++)
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

		bool Renderer::getWindowCloseEvent() {
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
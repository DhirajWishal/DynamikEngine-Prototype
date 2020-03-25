#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019

 vulkanRenderer.h file
*/

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_H

#include "core/base/RendererBackendBase.h"

#include "GameObject.h"
#include "debugger.h"

#include "core/data store/internalFormat.h"

#ifdef DMK_USE_VULKAN
namespace Dynamik {
	namespace ADGR {
		using namespace core;

		/* RENDERER BACKEND LAYER
		 * BASE: RendererBackend
		 * Vulkan Backend API.
		 * Derived from the RendererBackend super class.
		 * This class contains all the necessary functions, data types and objects to correctly
		  handle all Vulkan-based rendering tasks.
		  *
		  * @warn: Only one instance should be created.
		 */
		class vulkanRenderer : public RendererBackend {
			/* DEFAULTS */
		public:
			/* CONSTRUCTOR
			 * Default constructor of the Vulkan Renderer.
			 */
			vulkanRenderer() {}

			/* CONSTRUCTOR
			 * Default constructor of the Vulkan Renderer.
			 *
			 * @param settings: Renderer Backend settings container.
			 */
			vulkanRenderer(DMKRendererSettings settings) : RendererBackend(settings) {}

			/* DESTRUCTOR
			 * Default destructor of the Vulkan Renderer.
			 */
			~vulkanRenderer() {}

			/* PUBLIC FUNCTIONS */
		public:
			/* FUNCTION
			 * Full initialization function.
			 */
			void init() override;

			/* FUNCTION
			 * Stage One initialization.
			 */
			void initStageOne() override;

			/* FUNCTION
			 * Stage Two initialization.
			 */
			void initStageTwo() override;

			/* FUNCTION
			 * Stage Three initialization.
			 */
			void initStageThree() override;

			/* FUNCTION
			 * Draw frame function.
			 */
			void drawFrame() override;

			/* FUNCTION
			 * Full Shut down function.
			 */
			void shutDown() override;

			/* FUNCTION
			 * Shut down stage one.
			 */
			void shutDownStageOne() override;

			/* FUNCTION
			 * Shut down stage two.
			 */
			void shutDownStageTwo() override;

			/* FUNCTION
			 * Shut down stage three.
			 */
			void shutDownStageThree() override;

			/* FUNCTION
			 * Get events.
			 */
			std::deque<DMKEventContainer>* events() override;

			/* FUNCTION
			 * Check for window close event.
			 */
			inline B1 closeEvent() override { return false; }

			/* FUNCTION
			 * Set Renderer formats to the renderer.
			 *
			 * @param rendererFormats: An ARRAY of renderer formats.
			 */
			void setFormats(ARRAY<RendererFormat>& rendererFormats) override;

			/* FUNCTION
			 * Update the renderer formats.
			 *
			 * @param rendererFormats: An ARRAY of renderer formats.
			 */
			void updateFormats(ARRAY<RendererFormat>& rendererFormats) override;

			/* PRIVATE FUNCTIONS */
		private:
			void recreateSwapChain();
		};
	}
}

#endif
#endif	//	_DMK_ADGR_RENDER_H

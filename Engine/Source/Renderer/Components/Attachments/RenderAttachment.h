#pragma once
#ifndef _DYNAMIK_RENDERER_RENDER_ATTACHMENT_H
#define _DYNAMIK_RENDERER_RENDER_ATTACHMENT_H

#include "../../Backend/Primitives/FrameBuffer.h"
#include "../../Backend/Primitives/IndexBuffer.h"
#include "../../Backend/Primitives/Pipeline.h"
#include "../../Backend/Primitives/PipelineCache.h"
#include "../../Backend/Primitives/RenderPass.h"
#include "../../Backend/Primitives/SwapChain.h"
#include "../../Backend/Primitives/TextureData.h"
#include "../../Backend/Primitives/VertexBuffer.h"

namespace Dynamik {
	namespace Renderer {
		/* Dynamik Rendering Attachments
		 * Rendering attachments are components/ objects that are'nt directly rendererd by the renderer.
		 * Instead it is used as a supporting structure by scene components.
		 */
		class RenderAttachment {
		public:
			RenderAttachment() {}
			virtual ~RenderAttachment() {}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_RENDER_ATTACHMENT_H

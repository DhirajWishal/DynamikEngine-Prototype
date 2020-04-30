#include "dmkafx.h"
#include "StaticObject.h"

#include "../../../Backend/Vulkan/VulkanPresets.h"

namespace Dynamik {
	namespace ADGR {
		using namespace Backend;

		void StaticObject::initializeResources()
		{
			/* Generate Vertex Buffers, Index Buffers and Textures  */
			for (auto _mesh : myInternalFormat->meshDatas)
			{
				ADGR::RenderingMeshComponent _component;
				_component.vertexBuffer = ADGR::Backend::VulkanRBL::initializeVertexBuffer(_mesh, myInternalFormat->descriptor.vertexBufferObjectDescription.attributes).get();
				_component.indexBuffer = ADGR::Backend::VulkanRBL::initializeIndexBuffer(_mesh, myInternalFormat->descriptor.indexBufferType).get();

				for (auto _texture : _mesh.textureDatas)
					_component.textureData = ADGR::Backend::VulkanRBL::initializeTextureData(_texture).get();

				myMeshes.pushBack(_component);
			}

			/* Initialize uniform buffers */
			for (auto description : myInternalFormat->descriptor.uniformBufferObjectDescriptions)
				if (description.type == DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT)
					ADGR::Backend::VulkanRBL::initializeUniformBuffer(DMKUniformBufferObjectDescriptor::uniformByteSize(description.attributes));

			/* Resolve Attachments */
			for (auto attachment : myInternalFormat->descriptor.renderSpecification.renderAttachments)
			{
				switch (attachment)
				{
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_SKYBOX:
					break;
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_LIGHTING:
					break;
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_IRRADIANCE_CUBE:
					break;
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_PREFILTERED_CUBE:
					break;
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_BRDF_TABLE:
					break;
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_COMPUTE_TEXTURE:
					break;
				}
			}

			/* Pipeline creation */
			myPipeline = ADGR::Backend::VulkanRBL::initializePipeline(VulkanPresets::)
		}

		void StaticObject::initializeContext()
		{
		}

		DMKUniformBufferData StaticObject::update(DMKCameraData cameraData)
		{
			return DMKUniformBufferData();
		}

		void StaticObject::terminate()
		{
		}
	}
}
#include "dmkafx.h"
#include "StaticObject.h"

namespace Dynamik {
	namespace ADGR {
		void StaticObject::initializeResources()
		{			
			/* Generate Vertex Buffers, Index Buffers and Textures  */
			for (auto _mesh : myInternalFormat->meshDatas)
			{
				ADGR::RenderingMeshComponent _component;
				_component.vertexBuffer = ADGR::Backend::VulkanRBL::initializeVertexBuffer(_mesh, myInternalFormat->descriptor.vertexBufferObjectDescription.attributes);
				_component.indexBuffer = ADGR::Backend::VulkanRBL::initializeIndexBuffer(_mesh, myInternalFormat->descriptor.indexBufferType);

				if (myInternalFormat->descriptor.renderSpecification.renderAttachments.find(DMKRenderAttachment::DMK_RENDER_ATTACHMENT_TEXTURE).size())
					for (auto _texture : _mesh.textureDatas)
						_component.textureData = ADGR::Backend::VulkanRBL::initializeTextureData(_texture);

				myMeshes.pushBack(_component);
			}

			/* Resolve Attachments */
			for (auto attachment : myInternalFormat->descriptor.renderSpecification.renderAttachments)
			{
				switch (attachment)
				{
				case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_UNIFORM_BUFFER:
					for (auto description : myInternalFormat->descriptor.uniformBufferObjectDescriptions)
						if (description.type == DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT)
							ADGR::Backend::VulkanRBL::initializeUniformBuffer(DMKUniformBufferObjectDescriptor::uniformByteSize(description.attributes));
					break;
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
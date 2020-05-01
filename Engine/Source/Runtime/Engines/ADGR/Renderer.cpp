/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#include "dmkafx.h"
#include "Renderer.h"
#include "defines.h"

#include "debugger.h"

#include "Platform/windows.h"

#include "Backend/Vulkan/VulkanRBL.h"
#include "Backend/Vulkan/VulkanPresets.h"

namespace Dynamik {
	namespace ADGR {
		/* RBL declaration */

		/* Renderer instance definition */
		Renderer Renderer::instance;

		void Renderer::initializeStageOne(DMKRenderingAPI selectedAPI, DMKRendererSettings settings)
		{
			switch (selectedAPI)
			{
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_VULKAN:
				break;
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_OPENGL:
				break;
			case Dynamik::ADGR::DMKRenderingAPI::DMK_RENDERING_API_DIRECTX_12:
				break;
			default:
				DMK_CORE_FATAL("Unsupported Rendering API!");
				break;
			}

			VulkanRenderer::initializeStageOne();
		}

		void Renderer::initializeRenderContext(RenderContextType type)
		{
			instance.defaultContext = VulkanRenderer::createContext(type, instance.attachments);
		}

		void Renderer::initializeAttachment(DMKRenderAttachment attachmentType)
		{
			switch (attachmentType)
			{
			case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_COLOR:
				instance.attachments.pushBack(VulkanRenderer::generateColorAttachment(instance.defaultContext.swapChain));
				instance.attachmentIndex[DMKRenderAttachment::DMK_RENDER_ATTACHMENT_COLOR] = instance.attachments.size() - 1;
				break;
			case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_DEPTH:
				instance.attachments.pushBack(VulkanRenderer::generateDepthAttachment());
				instance.attachmentIndex[DMKRenderAttachment::DMK_RENDER_ATTACHMENT_DEPTH] = instance.attachments.size() - 1;
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
			case Dynamik::DMKRenderAttachment::DMK_RENDER_ATTACHMENT_CUSTOM:
				break;
			}
		}

		void Renderer::initializeStageTwo()
		{
		}

		void Renderer::initializeStageThree()
		{
		}

		void Renderer::setProgressPointer(POINTER<UI32> progress)
		{
			instance.progressPtr = progress;
		}

		void Renderer::setWindowHandle(POINTER<GLFWwindow> window)
		{
			VulkanRenderer::setWindowHandle(window);
		}

		void Renderer::setWindowExtent(UI32 width, UI32 height)
		{
			VulkanRenderer::setWindowExtent(width, height);
		}

		void Renderer::setRenderableObjects(ARRAY<POINTER<InternalFormat>> formats, RenderContextType context)
		{
			DMK_BEGIN_PROFILE_TIMER();

			for (auto format : formats)
			{
				switch (format->type)
				{
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_IMAGE_2D:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DEBUG_OBJECT:
					initializeDebugObject(format);
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_MESH:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_STATIC:
					initializeStaticObject(format);
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_INTERACTIVE_OBJECT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PLAYER:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_NPC:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXTURE_UI:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKYBOX:
					initializeSkyBoxObject(format);
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SPRITES:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_FONT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_TEXT_OVERLAY:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_PARTICLE:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTION:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_DESTRUCTOR:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SKELETAL_ANIMATION:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_LIGHT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_CAMERA:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AUDIO:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_VIDEO:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_WIND:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_AI:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_SCRIPT:
					break;
				case Dynamik::DMKObjectType::DMK_OBJECT_TYPE_KEY_BINDINGS:
					break;
				}

				instance.submitPendingAssets.pushBack(format);
			}
		}

		void Renderer::submitLoadedAssets()
		{
			DMK_BEGIN_PROFILE_TIMER();
			instance.submitPendingAssets = {};
		}

		void Renderer::addToRenderQueue(POINTER<InternalFormat> container)
		{
			instance.submitPendingAssets.pushBack(container);
		}

		void Renderer::drawFrame(DMKRendererDrawFrameInfo info)
		{
		}

		void Renderer::frameCleanup()
		{
			DMK_BEGIN_PROFILE_TIMER();

		}

		void Renderer::terminate()
		{
			DMK_BEGIN_PROFILE_TIMER();

		}

		ADGR::StaticObject Renderer::initializeStaticObject(POINTER<InternalFormat> format)
		{
			ADGR::StaticObject _object;
			_object.myInternalFormat = format;

			/* Generate Vertex Buffers, Index Buffers and Textures  */
			for (auto _mesh : _object.myInternalFormat->meshDatas)
			{
				ADGR::RenderingMeshComponent _component;
				_component.vertexBuffer = ADGR::Backend::VulkanRBL::initializeVertexBuffer(_mesh, _object.myInternalFormat->descriptor.vertexBufferObjectDescription.attributes).get();
				_component.indexBuffer = ADGR::Backend::VulkanRBL::initializeIndexBuffer(_mesh, _object.myInternalFormat->descriptor.indexBufferType).get();

				for (auto _texture : _mesh.textureDatas)
					_component.textureData = ADGR::Backend::VulkanRBL::initializeTextureData(_texture).get();

				_object.myMeshes.pushBack(_component);
			}

			/* Initialize uniform buffers */
			for (auto description : _object.myInternalFormat->descriptor.uniformBufferObjectDescriptions)
				if (description.type == DMKUniformType::DMK_UNIFORM_TYPE_BUFFER_OBJECT)
					ADGR::Backend::VulkanRBL::initializeUniformBuffer(DMKUniformBufferObjectDescriptor::uniformByteSize(description.attributes));

			/* Resolve Attachments */
			for (auto attachment : _object.myInternalFormat->descriptor.renderSpecification.renderAttachments)
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
			_object.myPipeline = ADGR::Backend::VulkanRBL::initializePipeline(
				Backend::VulkanPresets::pipelinePreset3D(
					format->descriptor.vertexBufferObjectDescription.attributes,
					RenderUtilities::getShaders(format->shaderPaths)));

			POINTER<StaticObject> _sceneComponent = StaticAllocator<StaticObject>::allocate();
			StaticAllocator<StaticObject>::set(_sceneComponent, (StaticObject&&)_object);
			instance.sceneComponents.pushBack(_sceneComponent);
		}
		
		ADGR::SkyBoxObject Renderer::initializeSkyBoxObject(POINTER<InternalFormat> format)
		{
			return ADGR::SkyBoxObject();
		}
		
		ADGR::DebugObject Renderer::initializeDebugObject(POINTER<InternalFormat> format)
		{
			return ADGR::DebugObject();
		}
		
		ARRAY<Shader> Renderer::RenderUtilities::getShaders(ShaderPaths paths)
		{
			ARRAY<Shader> _shaders;

			if (paths.vertexShader.size() && paths.vertexShader != "NONE")
			{
				Shader _shader(DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX, paths.vertexShader);
			}
			else if (paths.tessellationShader.size() && paths.tessellationShader != "NONE")
			{
				Shader _shader(DMKShaderLocation::DMK_SHADER_LOCATION_TESSELLATION, paths.tessellationShader);
			}
			else if (paths.geometryShader.size() && paths.geometryShader != "NONE")
			{
				Shader _shader(DMKShaderLocation::DMK_SHADER_LOCATION_GEOMETRY, paths.geometryShader);
			}
			else if (paths.fragmentShader.size() && paths.fragmentShader != "NONE")
			{
				Shader _shader(DMKShaderLocation::DMK_SHADER_LOCATION_FRAGMENT, paths.fragmentShader);
			}
			else
				DMK_CORE_ERROR("Invalid shader type!");
		}
	}
}
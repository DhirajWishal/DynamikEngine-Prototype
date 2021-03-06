#pragma once
#ifndef _DYNAMIK_RENDERER_SHADER_H
#define _DYNAMIK_RENDERER_SHADER_H

#include "RenderPrimitive.h"
#include "Dynamik/objectSpecifications.h"

namespace Dynamik {
	namespace Renderer {
		class Shader : public RenderPrimitive {
		public:
			Shader() : RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SHADER) {}
			Shader(DMKShaderLocation loc) :
				RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SHADER),
				location(loc) {}
			Shader(std::string path) : 
				RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SHADER), 
				path(path) {}
			Shader(DMKShaderLocation loc, std::string path) :
				RenderPrimitive(DMKRenderPrimitiveType::DMK_RENDER_PRIMITIVE_TYPE_SHADER),
				location(loc),
				path(path) {}
			virtual ~Shader() {}

			std::string path;
			DMKShaderLocation location = DMKShaderLocation::DMK_SHADER_LOCATION_VERTEX;
			std::vector<CHR> code;

		protected:
			std::vector<CHR> getCode(std::string path)
			{
				std::ifstream file(path, std::ios::ate | std::ios::binary);

				if (!file.is_open())
					throw std::runtime_error("Failed to Open file!");

				size_t fileSize = (size_t)file.tellg();
				std::vector<char> buffer(fileSize);
				file.seekg(0);
				file.read(buffer.data(), fileSize);

				file.close();
				return buffer;
			}
		};
	}
}

#endif // !_DYNAMIK_RENDERER_SHADER_H

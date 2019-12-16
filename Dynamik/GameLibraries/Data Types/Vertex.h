#pragma once

#ifndef _DYNAMIK_VERTEX_H
#define _DYNAMIK_VERTEX_H

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "core/data store/objectSpecifications.h"

namespace Dynamik {
	struct DMKVertexInputBindingDescription {
		uint32_t binding = 0;
		uint32_t stride = 0;
	};

	struct DMKVertexInputAttributeDescription {
		uint32_t binding = 0;
		uint32_t location = 0;
		DMKFormat format;
		uint32_t offset = 0;
	};

	class VertexDataContainer {
	public:
		struct Vertex {
			virtual bool operator==(const Vertex& other) const {}
		};

		virtual std::vector<DMKVertexInputBindingDescription> getVertexInputBindingDescription(int bindCount) {
			return std::vector<DMKVertexInputBindingDescription>();
		}
		virtual std::vector<DMKVertexInputAttributeDescription> getVertexInputAttributeDescription() {
			return std::vector<DMKVertexInputAttributeDescription>();
		}
	};
}

#endif

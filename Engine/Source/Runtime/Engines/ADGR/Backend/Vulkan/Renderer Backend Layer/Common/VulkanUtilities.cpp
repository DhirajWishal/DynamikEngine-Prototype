#include "dmkafx.h"
#include "VulkanUtilities.h"

#include "GameObjectDescriptors.h"

namespace Dynamik {
	namespace ADGR {
		namespace Backend {
			ARRAY<VkVertexInputBindingDescription> VulkanUtilities::getBindingDescription(ARRAY<DMKVertexAttribute> attributes, UI32 bindCount)
			{
				ARRAY<VkVertexInputBindingDescription> bindingDescription(bindCount);

				for (int i = 0; i < bindCount; i++) {
					bindingDescription[i].binding = i;
					bindingDescription[i].stride = DMKVertexBufferObjectDescriptor::vertexByteSize(attributes);
					bindingDescription[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				}

				return bindingDescription;
			}

			ARRAY<VkVertexInputAttributeDescription> VulkanUtilities::getAttributeDescriptions(ARRAY<DMKVertexAttribute> attributes, UI32 binding)
			{
				ARRAY<VkVertexInputAttributeDescription> attributeDescriptions;
				UI32 _previousTypeSize = 0;

				for (UI32 _index = 0; _index < attributes.size(); _index++)
				{
					VkVertexInputAttributeDescription _description = {};
					_description.binding = binding;
					_description.location = _index;
					_description.format = vertexAttributeTypeToVkFormat(attributes[_index].type);
					_description.offset = _previousTypeSize;
					attributeDescriptions.pushBack(_description);

					_previousTypeSize += (UI32)attributes[_index].type;
				}

				return attributeDescriptions;
			}
			
			VkFormat VulkanUtilities::vertexAttributeTypeToVkFormat(DMKDataType type)
			{
				const UI32 _baseByteSize = sizeof(F32);

				switch ((UI32)type)
				{
				case (_baseByteSize * 1):
					return VkFormat::VK_FORMAT_R32_SFLOAT;
					break;

				case (_baseByteSize * 2):
					return VkFormat::VK_FORMAT_R32G32_SFLOAT;
					break;

				case (_baseByteSize * 3):
					return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
					break;

				case (_baseByteSize * 4):
					return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
					break;

				case (_baseByteSize * 9):
					return VkFormat::VK_FORMAT_R32_SFLOAT;
					break;

				case (_baseByteSize * 16):
					return VkFormat::VK_FORMAT_R32_SFLOAT;
					break;
				}

				return VkFormat::VK_FORMAT_UNDEFINED;
			}
		}
	}
}
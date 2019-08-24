#pragma once

#include "Core.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKGenerateMipMapInfo {
				VkFormat imageFormat;
				VkImage textureImage;

				int width;
				int height;

				uint32_t mipLevels;
			};

			class Texture : public Core {
			public:
				Texture() {}
				virtual ~Texture() {}

				virtual void loadTexture(std::string path) {}
				virtual void generateMipMaps(DMKGenerateMipMapInfo info) {}

			};
		}
	}
}

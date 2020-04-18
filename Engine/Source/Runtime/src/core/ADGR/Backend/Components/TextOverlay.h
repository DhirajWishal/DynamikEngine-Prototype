#pragma once
#ifndef _DYNAMIK_ADGR_TEXT_OVERLAY_H
#define _DYNAMIK_ADGR_TEXT_OVERLAY_H

namespace Dynamik {
	namespace ADGR {
		enum class DMKTextAlign {
			DMK_TEXT_ALIGN_LEFT,
			DMK_TEXT_ALIGN_RIGHT,
			DMK_TEXT_ALIGN_CENTER
		};

		class TextOverlay {
		public:
			TextOverlay() {}
			virtual ~TextOverlay() {}

			virtual void update(std::string text, F32 x, F32 y, DMKTextAlign align) {}
		};
	}
}

#endif // !_DYNAMIK_ADGR_TEXT_OVERLAY_H

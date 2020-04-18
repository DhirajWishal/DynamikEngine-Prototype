#include "dmkafx.h"
#include "event.h"

namespace Dynamik {
	template<typename TN>
	bool EventDispatcher::dispatch(eventFunc<TN> func) {
		if (ed_Event.getEventType() == TN::getStaticType()) {
			ed_Event.handled = func(*(TN*)&ed_Event);
			return true;
		}

		return false;
	}
}
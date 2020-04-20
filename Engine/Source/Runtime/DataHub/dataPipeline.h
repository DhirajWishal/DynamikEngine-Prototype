#pragma once
#ifndef _DYNAMIK_CENTRAL_DATA_HUB_DATA_PIPELINE_H
#define _DYNAMIK_CENTRAL_DATA_HUB_DATA_PIPELINE_H

#include <vector>
#include "Public/datatypes.h"

namespace Dynamik {
	class DataPipeline {
	public:
		DataPipeline() {}
		~DataPipeline() {}

		void addData(VPTR data);
		VPTR getData(UI32 index);
		void deleteData(UI32 index);

	private:
		ARRAY<VPTR> dataAddresses = {};
		UI32 dataAddressesSize = 0;
	};
}

#endif // !_DYNAMIK_CENTRAL_DATA_HUB_DATA_PIPELINE_H

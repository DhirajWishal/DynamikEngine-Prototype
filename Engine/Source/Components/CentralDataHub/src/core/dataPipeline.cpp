#include "cdhafx.h"
#include "dataPipeline.h"

namespace Dynamik {
	void DataPipeline::addData(VPTR data) {
		dataAddresses.push_back(data);
		dataAddressesSize++;
	}

	VPTR DataPipeline::getData(UI32 index) {
		if (dataAddressesSize < index)
			return nullptr;
		if (dataAddresses[index] == nullptr)
			return nullptr;

		return dataAddresses[index];
	}

	void DataPipeline::deleteData(UI32 index) {
		if (dataAddressesSize < index)
			return;
		if (dataAddresses[index] == nullptr)
			return;

		dataAddresses[index] = nullptr;
	}
}
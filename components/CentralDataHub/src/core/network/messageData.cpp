#include "cdhafx.h"
#include "messageData.h"

namespace Dynamik {
	/* PUBLIC MEMBERS */
	MessageData::MessageData(void* data) : m_data(data) {
	}

	template<typename Alloc>
	MessageData::MessageData(Alloc data) : m_data(&data) {
	}

	MessageData::~MessageData() {
		m_data = nullptr;
	}

	template<typename Alloc>
	Alloc MessageData::getData() {
		return *(Alloc*)m_data;
	}

	template<typename T>
	MessageData& MessageData::operator=(const T& value) {
		this->_set(&value);
		return *this;
	}

	MessageData& MessageData::operator=(MessageData& value) {
		this->_set(&value);
		return *this;
	}

	/* PRIVATE MEMBERS */
	void MessageData::_set(const void* data) {
		m_data = (void*)data;
	}
}

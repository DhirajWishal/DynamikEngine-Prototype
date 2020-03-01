#pragma once

#ifndef _DYNAMIK_NETWORING_CLIENT_H
#define _DYNAMIK_NETWORING_CLIENT_H

#include "client/Client.h"

namespace Dynamik {
	namespace Networking {
		class DMK_API client : public Client {
		public:
			client() {}
			~client() {}

			int connect(std::wstring& ipAddress, int portNumber) override;

		private:
		};
	}
}

#endif

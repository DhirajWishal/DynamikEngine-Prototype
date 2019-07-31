#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

namespace Dynamik {
	namespace ADGR {
		namespace core {

			class instance {
			public:
				instance(VkInstance* instance);

				void createInstance();
				void deleteInstance();

			private:
				VkInstance* myInstance;
			};
		}
	}
}

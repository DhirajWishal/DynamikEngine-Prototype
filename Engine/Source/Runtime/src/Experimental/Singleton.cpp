#include "dmkafx.h"
#include "Singleton.h"

namespace Dynamik {
	namespace Experimental {
		Singleton& Singleton::get()
		{
			static Singleton myInstance;
			return myInstance;
		}
		
		void Singleton::printHello()
		{
			get()._hello();
		}
		
		void Singleton::_hello()
		{
			DMK_CORE_INFO("Hello world");
		}
	}
}
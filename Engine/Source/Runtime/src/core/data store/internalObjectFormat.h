#pragma once

/*
 Internal Object Format for the Dynamik Engine

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		08/012/2019
 IDE:		MS Visual Studio Community 2019

 internalObjectFormat.h file
*/

#ifndef _DYNAMIK_CENTRAL_DATA_HUB_INTERNAL_OBJECT_FORMAT_H
#define _DYNAMIK_CENTRAL_DATA_HUB_INTERNAL_OBJECT_FORMAT_H

#include <GameObject.h>

namespace Dynamik {
	/*
	 INTERNAL OBJECT FORMAT
	 This format(class) is used locally inside the Dynamik Engine to handle data and resources
	 of each object. This class is only accessible within the Engine. User only gets to handle
	 the GameObject class to pass in object data;
	*/
	class InternalObjectFormat : public GameObject {
	public:
		InternalObjectFormat() {}
		virtual ~InternalObjectFormat() {}
	};
}

#endif

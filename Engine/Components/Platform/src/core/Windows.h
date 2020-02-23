#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_PLATFORM_WINDOWS_H
#define _DYNAMIK_PLATFORM_WINDOWS_H

#ifdef DMK_PLATFORM_WINDOWS
#include "Windows/window.h"
#include "Windows/inputs.h"

#include "Windows/utils/readFile.h"
#include "Windows/utils/loadIcon.h"
#include "Windows/utils/daiFileManager.h"
#include "Windows/utils/objectLoader.h"
#include "Windows/utils/modelManager.h"

#endif

#endif // !_DYNAMIK_PLATFORM_WINDOWS_H

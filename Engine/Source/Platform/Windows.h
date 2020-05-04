#pragma once

/*
 Core objects and functions of the Advanced Dynamic Graphics Renderer

 Author:	Dhiraj Wishal
 Project:	Dynamik
 Date:		30/07/2019
 IDE:		MS Visual Studio Community 2019
*/

#ifndef _DYNAMIK_PLATFORM_WINDOWS_H
#define _DYNAMIK_PLATFORM_WINDOWS_H

#ifdef DMK_PLATFORM_WINDOWS
#include "Windows/Window/WindowManager.h"

#include "Windows/file/daiFileManager.h"
#include "Windows/file/duiFileManager.h"
#include "Windows/file/fileManager.h"
#include "Windows/file/readFile.h"

#include "Windows/resource/imageLoader.h"
#include "Windows/resource/loadIcon.h"
#include "Windows/resource/modelManager.h"
#include "Windows/resource/objectLoader.h"

#include "Windows/thread/ExecutionHandle.h"
#include "Windows/thread/InternalThread.h"
#include "Windows/thread/threadManager.h"

#endif

#endif // !_DYNAMIK_PLATFORM_WINDOWS_H

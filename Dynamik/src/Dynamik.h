#pragma once

/*
 Dynamik Engine main header.

 Author:	Dhiraj Wishal
 Project:	Dynamik Engine
 Date:		15/6/2019	(DEVELOPMENTS ONGOING)
 IDE:		MS Visual Studio Community 2019

 Dynamik.h file.
*/

/* ---------- THIS HEADER FILE IS FOR CLIENT USE ---------- */

/* STD INCLUDES(PRE-COMPILED HEADER) */
#include "dmkafx.h"

/* EXTERNAL INCLUDES */

/* CORE INCLUDES */
#include "core/application.h"
#include "core/core.h"
#include "core/log.h"

#include "core/Managers/MemoryManager.h"

#include "core/utils/DMK_DataTypes.h"
#include "core/utils/DMK_Functions.h"
#include "core/utils/DMK_Error.h"
#include "core/utils/DMK_Math.h"
#include "core/utils/safeLock.h"
#include "core/utils/dataStructures/heap.h"
#include "core/utils/dataStructures/linkedLists.h"

#include <GameObject/GameObject.h>

/* ENTRY POINT */
#include "core/entryPoint.h"

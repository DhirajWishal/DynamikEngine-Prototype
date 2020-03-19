#pragma once

#ifndef _DYNAMIK_NETWORKING_PCH
#define _DYNAMIK_NETWORKING_PCH

#include "DataTypesLib/DataTypesLib.h"
#include "MemoryLib/MemoryLib.h"
#include "SystemLib/SystemLib.h"
#include "AlgorithmLib/AlgorithmLib.h"

#include "CentralDataHub.h"
#include "Managers.h"
#include "Utilities.h"
#include "Platform.h"

#include <iostream>
#include <vector>
#include <string>

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#endif

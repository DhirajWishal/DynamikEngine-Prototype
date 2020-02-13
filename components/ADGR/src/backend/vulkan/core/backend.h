#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_BACKEND_H
#define _DYNAMIK_ADGR_VULKAN_BACKEND_H
#ifdef DMK_USE_VULKAN

#include "windowManager.h"
#include "instanceManager.h"
#include "deviceManager.h"
#include "swapChainManager.h"
#include "pipelineManager.h"
#include "uniformBufferManager.h"
#include "shaderManager.h"
#include "commandBufferManager.h"
#include "colorBufferManager.h"
#include "depthBufferManager.h"
#include "frameBufferManager.h"
#include "textureManager.h"
#include "glTexture.h"
#include "vertexBufferManager.h"
#include "indexBufferManager.h"
#include "skyboxManager.h"

#include "manager.h"

#include "others/syncObjects.h"

#include "validators/debugger/debugger.h"

#include "keyEvent.h"
#include "mouseEvent.h"

#include "vulkanFormat.h"

#endif // DMK_USE_VULKAN

#endif // !_DYNAMIK_ADGR_VULKAN_BACKEND_H

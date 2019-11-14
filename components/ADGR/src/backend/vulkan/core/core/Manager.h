#pragma once

#ifndef _DYNAMIK_ADGR_VULKAN_RENDERER_MANAGER_H
#define _DYNAMIK_ADGR_VULKAN_RENDERER_MANAGER_H

#include "Core.h"

#define _SET_2D_VECTOR(x)	std::vector<std::vector<x>>

namespace Dynamik {
	namespace ADGR {
		namespace core {
			class Manager : Core {
				Manager() {}
				virtual ~Manager() {}
			};

			/*
			//enum MANAGER_PIPELINE {
			//	NONE,
			//	DMK_ADGR_PIPELINE_2D,
			//	DMK_ADGR_PIPELINE_3D,
			//	DMK_ADGR_PIPELINE_SKYBOX
			//};
			//
			//enum MANAGER_SHADER_TYPE {
			//	NONE,
			//	DMK_ADGR_SHADER_VERTEX,
			//	DMK_ADGR_SHADER_TESSELLATION,
			//	DMK_ADGR_SHADER_GEOMETRY,
			//	DMK_ADGR_SHADER_FRAGMENT
			//};

			enum MANAGER_TYPE {
				// pipelines
				DMK_ADGR_PIPELINE_2D,
				DMK_ADGR_PIPELINE_3D,
				DMK_ADGR_PIPELINE_SKYBOX,

				// shaders
				DMK_ADGR_SHADER_VERTEX,
				DMK_ADGR_SHADER_TESSELLATION,
				DMK_ADGR_SHADER_GEOMETRY,
				DMK_ADGR_SHADER_FRAGMENT
			};

			struct ADGRManagerGetResourceInfo {
				MANAGER_TYPE type = {};
				int index = 0;

				void* data = nullptr;

				std::string path = "";
			};

			class Manager : Core {
			public:
				Manager() {}
				virtual ~Manager() {}

				// window
				virtual void addWindow(int index, GLFWwindow* window) {}
				virtual GLFWwindow* getWindow(int index) { return nullptr; }

				// instance
				virtual void addInstance(int index, VkInstance* instance) {}
				virtual VkInstance* getInstance(int index) { return nullptr; }

				// device
				virtual void addDevice(int index, VkDevice* instance) {}
				virtual VkDevice* getDevice(int index) { return nullptr; }

				// physical device
				virtual void addPhysicalDevice(int index, VkPhysicalDevice* instance) {}
				virtual VkPhysicalDevice* getPhysicalDevice(int index) { return nullptr; }

				// swapchain
				virtual void addSwapchain(int index, VkSwapchainKHR* swapchain) {}
				virtual VkSwapchainKHR* getSwapchain(int index) { return nullptr; }

				//// pipeline
				//virtual void addPipeline(MANAGER_PIPELINE type, int index, VkPipeline* pipeline) {}
				//virtual VkPipeline* getPipeline(MANAGER_PIPELINE type, int index) { return nullptr; }
				//
				//// descriptor set layout
				//virtual void addDescriptorSetLayout(int index, VkDescriptorSetLayout* layout) {}
				//virtual VkDescriptorSetLayout* getDescriptorSetLayout(int index) { return nullptr; }
				//
				//// descriptor pools
				//virtual void addDescriptorPool(int index, VkDescriptorPool* descriptorPool) {}
				//virtual VkDescriptorPool* getDescriptorPool(int index) { return nullptr; }
				//
				//// shader paths
				//virtual void addShaderPath(MANAGER_SHADER_TYPE type, int index, std::string& path) {}
				//virtual std::string getShaderPath(MANAGER_SHADER_TYPE type, int index) { return nullptr; }
				//
				//// shader modules
				//virtual void addShaderModule(MANAGER_SHADER_TYPE type, int index, VkShaderModule* shaderModule) {}
				//virtual VkShaderModule* getShaderModule(MANAGER_SHADER_TYPE type, int index) { return nullptr; }
				//
				//// pipeline layout
				//virtual void addPipelineLayout(MANAGER_PIPELINE type, int index, VkPipelineLayout* layout) {}
				//virtual VkPipelineLayout* getPipelineLayout(MANAGER_PIPELINE type, int index) { return nullptr; }

				// command pool
				virtual void addCommandPool(int index, VkCommandPool* commandPool) {}
				virtual VkCommandPool* getCommandPool(int index) { return nullptr; }

				// global
				virtual void setResource(ADGRManagerGetResourceInfo info) {}
				virtual void* getResource() {}

			protected:
				_SET_2D_VECTOR(GLFWwindow*) windows = {};

				_SET_2D_VECTOR(VkInstance*) instances = {};

				_SET_2D_VECTOR(VkDevice*) devices = {};
				_SET_2D_VECTOR(VkPhysicalDevice*) physicalDevices = {};

				_SET_2D_VECTOR(VkSwapchainKHR*) swapchains = {};

				_SET_2D_VECTOR(VkPipeline*) pipelines = { {}, {}, {} };

				_SET_2D_VECTOR(VkDescriptorSetLayout*) descriptorSetLayouts = {};

				_SET_2D_VECTOR(std::string&) shaderPaths = { {}, {}, {}, {} };
				_SET_2D_VECTOR(VkShaderModule*) shaderModules = { {}, {}, {}, {} };
			};*/
		}
	}
}

#endif	// !_DYNAMIK_ADGR_VULKAN_RENDERER_MANAGER_H

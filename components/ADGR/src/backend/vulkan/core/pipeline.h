#pragma once

#include "core/Pipeline.h"

namespace Dynamik {
	namespace ADGR {
		namespace core {

			struct DMKPipelineInitInfo {
				std::vector<VkDescriptorSetLayout_T*> layouts;

				// primitive assembly info
				VkPrimitiveTopology inputAssemblyTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				VkBool32 inputAssemblyPrimitiveRestartEnable = VK_FALSE;

				// viewport info
				int viweportCount = 1;

				// scissor info
				int scissorCount = 1;
				std::vector<VkOffset2D> offsets = { { 0, 0 } };

				// rasterizer info
				VkBool32 rasterizerDepthClampEnable = VK_FALSE;
				VkBool32 rasterizerDiscardEnable = VK_FALSE;
				VkPolygonMode rasterizerPolygonMode = VK_POLYGON_MODE_FILL;
				float rasterizerLineWidth = 1.0f;
				VkCullModeFlagBits rasterizerCullMode = VK_CULL_MODE_BACK_BIT;
				VkFrontFace rasterizerFrontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
				VkBool32 rasterizerDepthBiasEnable = VK_FALSE;

				// multisampling info
				VkSampleCountFlagBits multisamplerMsaaSamples = VK_SAMPLE_COUNT_1_BIT;
				VkBool32 multisamplerSampleShadingEnable = VK_TRUE;
				float multisamplerMinSampleShading = 0.2f;

				// depth stencil info
				VkBool32 depthStencilEnable = VK_TRUE;
				VkBool32 depthStencilWriteEnable = VK_TRUE;
				VkCompareOp depthStencilCompareOp = VK_COMPARE_OP_LESS;
				VkBool32 depthStencilBoundsTestEnable = VK_FALSE;
				VkBool32 depthStencilTestEnable = VK_FALSE;

				// color blender info
				std::vector<VkColorComponentFlags> colorBlenderColorWriteMasks = {
					VK_COLOR_COMPONENT_R_BIT
					| VK_COLOR_COMPONENT_G_BIT
					| VK_COLOR_COMPONENT_B_BIT
					| VK_COLOR_COMPONENT_A_BIT };
				std::vector<VkBool32> colorBlenderBlendEnables = { VK_FALSE };

				VkBool32 colorBlendingLogicOpEnable = VK_FALSE;
				VkLogicOp colorBlendingLogicOp = VK_LOGIC_OP_COPY;
				int colorBlendingColorBlendCount = 1;
				std::vector<float> colorBlendingBlendConstants = { 0.0f, 0.0f, 0.0f, 0.0f };

				// push constants info
				bool pushConstantsEnable = false;
				int pushConstantCount = 1;
				int pushConstantOffset = 0;

				// pipeline layout info
				VkPipelineLayout* pipelineLayout = nullptr;

				// dynamic state info
				bool dynamicStateEnable = false;

				// pipeline info
				int pipelineStageCount = 2;
				int pipelineSubPass = 0;
				VkPipeline pipelineBasePipelineHandle = VK_NULL_HANDLE;
				VkPipeline* pipeline = nullptr;
			};

			// TODO: Pipeline cashe

			class pipeline : public Pipeline {
			public:
				pipeline() {}
				~pipeline() {}

				void init(DMKPipelineInitInfo info);
				void initRenderPass() override;

				VkPipeline* getPipeline() { return m_pipeline; }
				VkPipelineLayout* getPipelineLayout() { return m_pipelineLayout; }

			private:
				VkDevice* m_device = &device;
				VkPipeline* m_pipeline = &graphicsPipeline;
				VkPipelineLayout* m_pipelineLayout = &pipelineLayout;
			};
		}
	}
}

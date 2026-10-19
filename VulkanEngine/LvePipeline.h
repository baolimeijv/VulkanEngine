#pragma once

#include"LveDevice.h"

#include<string>
#include<vector>
#include <cstdint>

namespace lve
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;

		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
      
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class LvePipeline
	{
	public:
		LvePipeline(LveDevice&Device,const std::string& VertFilepath, const std::string& fragFilepath,const PipelineConfigInfo&ConfigInfo);
		~LvePipeline();

		LvePipeline(const LvePipeline&) = delete;
		LvePipeline operator=(const LvePipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);

		void static defaultPipelineConfigInfo(PipelineConfigInfo& ConfigInfo);
		
	private:
		LveDevice& lveDevice;
		VkPipeline GraphicsPipline;
		VkShaderModule vertShaderMoudule;
		VkShaderModule fragShaderMoudule;

		static std::vector<char> ReadFile(const std::string& Filepath);

		void CreateGraphicsPipline(const std::string& VertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& ConfigInfo);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* ShaderModule);
	};


}
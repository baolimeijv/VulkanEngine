#include"LvePipeline.h"
#include"LveModel.h"

#include<iostream>
#include<fstream>
#include<cassert>
namespace lve
{
	LvePipeline::LvePipeline(LveDevice& Device, const std::string& VertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& ConfigInfo):lveDevice(Device)
	{
		CreateGraphicsPipline(VertFilepath, fragFilepath,ConfigInfo);
	}

	LvePipeline::~LvePipeline()
	{
		vkDestroyShaderModule(lveDevice.device(), vertShaderMoudule, nullptr);
		vkDestroyShaderModule(lveDevice.device(), fragShaderMoudule, nullptr);
		vkDestroyPipeline(lveDevice.device(), GraphicsPipline, nullptr);
	}

	void LvePipeline::bind(VkCommandBuffer commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipline);
	}

	void LvePipeline::defaultPipelineConfigInfo(PipelineConfigInfo& ConfigInfo)
	{
		ConfigInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		ConfigInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		ConfigInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		ConfigInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		ConfigInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		ConfigInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		ConfigInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		ConfigInfo.rasterizationInfo.lineWidth = 1.0f;
		ConfigInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		ConfigInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		ConfigInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		ConfigInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		ConfigInfo.rasterizationInfo.depthBiasClamp = 0.0f;
		ConfigInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		ConfigInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		ConfigInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		ConfigInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		ConfigInfo.multisampleInfo.minSampleShading = 1.0f;
		ConfigInfo.multisampleInfo.pSampleMask = nullptr;
		ConfigInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		ConfigInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

		ConfigInfo.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		ConfigInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		ConfigInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		ConfigInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		ConfigInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		ConfigInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		ConfigInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		ConfigInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		ConfigInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		ConfigInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		ConfigInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		ConfigInfo.colorBlendInfo.attachmentCount = 1;
		ConfigInfo.colorBlendInfo.pAttachments = &ConfigInfo.colorBlendAttachment;
		ConfigInfo.colorBlendInfo.blendConstants[0] = 0.0f;
		ConfigInfo.colorBlendInfo.blendConstants[1]= 0.0f;
		ConfigInfo.colorBlendInfo.blendConstants[2] = 0.0f;
		ConfigInfo.colorBlendInfo.blendConstants[3] = 0.0f;

		ConfigInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		ConfigInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		ConfigInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		ConfigInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		ConfigInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		ConfigInfo.depthStencilInfo.minDepthBounds = 0.0f;
		ConfigInfo.depthStencilInfo.maxDepthBounds = 1.0f;
		ConfigInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		ConfigInfo.depthStencilInfo.front = {};
		ConfigInfo.depthStencilInfo.back = {};

		ConfigInfo.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT,VK_DYNAMIC_STATE_SCISSOR };

		ConfigInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		ConfigInfo.dynamicStateInfo.pDynamicStates = ConfigInfo.dynamicStateEnables.data();
		ConfigInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(ConfigInfo.dynamicStateEnables.size());

	}

	std::vector<char> LvePipeline::ReadFile(const std::string& Filepath)
	{
		std::ifstream file{ Filepath,std::ios::ate | std::ios::binary };
		if (!file.is_open())throw std::runtime_error("failed to open file:" + Filepath);

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char>buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void LvePipeline::CreateGraphicsPipline(const std::string& VertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& ConfigInfo)
	{
		assert(ConfigInfo.pipelineLayout != VK_NULL_HANDLE && "cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
		assert(ConfigInfo.renderPass != VK_NULL_HANDLE && "cannot create graphics pipeline:: no renderPass provided in configInfo");

		auto VertCode = ReadFile(VertFilepath);
		auto FragCode = ReadFile(fragFilepath);

		CreateShaderModule(VertCode, &vertShaderMoudule);
		CreateShaderModule(FragCode, &fragShaderMoudule);

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertShaderMoudule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragShaderMoudule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		auto bindingDescriptions = LveModel::Vertex::getBindingDescriptions();
		auto attributeDescriptions = LveModel::Vertex::getAttributeDescriptions();
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = nullptr;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = nullptr;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &ConfigInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &ConfigInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &ConfigInfo.multisampleInfo;
		pipelineInfo.pColorBlendState = &ConfigInfo.colorBlendInfo;
		pipelineInfo.pDepthStencilState = &ConfigInfo.depthStencilInfo;
		pipelineInfo.pDynamicState = &ConfigInfo.dynamicStateInfo;

		pipelineInfo.layout = ConfigInfo.pipelineLayout;
		pipelineInfo.renderPass = ConfigInfo.renderPass;
		pipelineInfo.subpass = ConfigInfo.subpass;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &GraphicsPipline) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics pipeline");
		}

	}

	void LvePipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* ShaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, ShaderModule) != VK_SUCCESS)
		{
			throw::std::runtime_error("failed to create shader module");
		}
	}
}
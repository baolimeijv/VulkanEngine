#include"SimpleRenderSystem.h"
#include<stdexcept>
#include<array>
#include<cassert>
#include"glm/gtc/constants.hpp"

namespace lve
{
	SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass):lveDevice(device)
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);

	}
	
	void SimpleRenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(VkPushConstantRange);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineInfo{};
		LvePipeline::defaultPipelineConfigInfo(pipelineInfo);
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(lveDevice, "SimpleShader.vert.spv", "SimpleShader.frag.spv", pipelineInfo);
	}

	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<LveGameObject>& gameObjects)
	{
		int i = 0;
		for (auto& object : gameObjects)
		{
			i++;
			object.transform.rotation.y = glm::mod<float>(object.transform.rotation.y + 0.0001f * i, 2.0f * glm::pi<float>());
			object.transform.rotation.x = glm::mod<float>(object.transform.rotation.x + 0.00005f * i, 2.0f * glm::pi<float>());
		}


		lvePipeline->bind(commandBuffer);

		for (auto& obj : gameObjects)
		{
			SimplePushConstantData push{};
			push.color = obj.color;
			push.transform = obj.transform.mat4();
			vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}
}
#include"FirstApp.h"
#include<stdexcept>
#include<array>
#include<cassert>
#include"glm/gtc/constants.hpp"

namespace lve
{
	FirstApp::FirstApp()
	{
		loadGameObjects();
		createPipelineLayout();
		createPipeline();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);

	}

	void FirstApp::run()
	{
		while (!Window.ShouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = lveRender.beginFrame())
			{
				lveRender.beginSwapChainRenderPass(commandBuffer);
				renderGameObjects(commandBuffer);
				lveRender.endSwapChainRenderPass(commandBuffer);
				lveRender.endFrame();

			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::vector<LveModel::Vertex>vertices
		{
			{{0.0f,-0.5f},{1.0f,0.0f,0.0f}},
			{{0.5f,0.5f},{0.0f,1.0f,0.0f}},
			{{-0.5,0.5f},{0.0f,0.0f,1.0f}}
		};
		auto lveModel = std::make_shared<LveModel>(lveDevice,vertices);
		auto triangle = LveGameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = { 0.1f,0.8f,0.1f };
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.scale = { 2.0f,0.2f };
		triangle.transform2d.rotation =0.25* glm::two_pi<float>();

		gameObject.push_back(std::move(triangle));
	}

	void FirstApp::createPipelineLayout()
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

	void FirstApp::createPipeline()
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineInfo{};
		LvePipeline::defaultPipelineConfigInfo(pipelineInfo);
		pipelineInfo.renderPass = lveRender.getSwapChainRenderPass();
		pipelineInfo.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(lveDevice, "SimpleShader.vert.spv", "SimpleShader.frag.spv", pipelineInfo);
	}

	void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer)
	{
		int i = 0;
		for (auto& object: gameObject)
		{
			i++;
			object.transform2d.rotation = glm::mod<float>(object.transform2d.rotation+0.001f*i,2.0f*glm::pi<float>());
		}


		lvePipeline->bind(commandBuffer);

		for (auto& obj : gameObject)
		{
			SimplePushConstantData push{};
			push.offset = obj.transform2d.translation;
			push.color = obj.color;
			push.transform = obj.transform2d.mat2();
			vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}
}
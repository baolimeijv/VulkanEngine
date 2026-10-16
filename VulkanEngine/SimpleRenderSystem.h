#pragma once

#include"LvePipeline.h"
#include"LveDevice.h"
#include"LveSwapChain.h"
#include"LveGameObject.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include"glm/glm.hpp"

#include<vector>
#include<memory>

struct SimplePushConstantData
{
	glm::mat4 transform{ 1.0f };
	alignas(16)glm::vec3 color;
};

namespace lve
{
	class SimpleRenderSystem
	{
	public:

		SimpleRenderSystem(LveDevice&device,VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer,std::vector<LveGameObject>&gameObjects);

	private:
		LveDevice &lveDevice;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
	};
}
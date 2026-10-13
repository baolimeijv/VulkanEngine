#pragma once

#include"LveWindow.h"
#include"LvePipeline.h"
#include"LveDevice.h"
#include"LveSwapChain.h"
#include"LveGameObject.h"
#include"LveRender.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include"glm/glm.hpp"

#include<vector>
#include<memory>

struct SimplePushConstantData
{
	glm::mat2 transform{1.0f};
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		LveWindow Window{ WIDTH,HEIGHT,"Hello Vulkan" };
		LveDevice lveDevice{ Window };
		LveRender lveRender{ Window,lveDevice };
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<LveGameObject> gameObject;

		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void renderGameObjects(VkCommandBuffer commandBuffer);
	};
}
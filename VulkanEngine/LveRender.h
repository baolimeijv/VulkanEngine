#pragma once
#pragma once

#include"LveWindow.h"
#include"LvePipeline.h"
#include"LveDevice.h"
#include"LveSwapChain.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include"glm/glm.hpp"

#include<vector>
#include<memory>
#include<cassert>

namespace lve
{
	class LveRender
	{
	public:
		LveRender(LveWindow&window,LveDevice &device);
		~LveRender();

		LveRender(const LveRender&) = delete;
		LveRender& operator=(const LveRender&) = delete;

		VkRenderPass getSwapChainRenderPass()const { return lveSwapChain->getRenderPass(); }

		bool isFrameInProgress()const { return isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer()const
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffer[currentFrameIndex];
		}

		int getFrameIndex()const 
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	private:
		LveWindow &Window;
		LveDevice &lveDevice;
		std::unique_ptr<LveSwapChain> lveSwapChain;
		std::vector<VkCommandBuffer> commandBuffer;

		bool isFrameStarted;
		uint32_t currentImageIndex;
		int currentFrameIndex;
		
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
	};
}
#include"FirstApp.h"
#include"SimpleRenderSystem.h"

#include<stdexcept>
#include<array>
#include<cassert>
#include"glm/gtc/constants.hpp"

namespace lve
{
	FirstApp::FirstApp()
	{
		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{

	}

	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ lveDevice,lveRender.getSwapChainRenderPass() };

		while (!Window.ShouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = lveRender.beginFrame())
			{
				lveRender.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer,gameObjects);
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

		gameObjects.push_back(std::move(triangle));
	}
}

	
	
#include"FirstApp.h"
#include"LveCamera.h"
#include"SimpleRenderSystem.h"

#include<stdexcept>
#include<array>
#include<cassert>
#include<chrono>

#include"glm/gtc/constants.hpp"

#define MAX_FRAME_TIME 0.1f

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
        SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRender.getSwapChainRenderPass() };
        LveCamera camera{};
        //camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
        camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.0f, 0.0f, 2.5f));

        auto viewerObject = LveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!Window.ShouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(Window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRender.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            if (auto commandBuffer = lveRender.beginFrame())
            {
                lveRender.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,camera);
                lveRender.endSwapChainRenderPass(commandBuffer);
                lveRender.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    

    void FirstApp::loadGameObjects()
    {
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
        auto cube = LveGameObject::createGameObject();
        cube.model = lveModel;
        cube.transform.translation = { 0.0f,0.0f,2.5f };
        cube.transform.rotation = { 0.0f,0.0f,0.0f };
        gameObjects.push_back(std::move(cube));


    }
}

	
	
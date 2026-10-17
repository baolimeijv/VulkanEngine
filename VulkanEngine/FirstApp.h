#pragma once

#include"LveWindow.h"
#include"LveDevice.h"
#include"LveSwapChain.h"
#include"LveGameObject.h"
#include"LveRender.h"
#include"KeyboardMovementController.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include"glm/glm.hpp"

#include<vector>
#include<memory>

#include <chrono>
#include <random>

namespace lve
{


    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        FirstApp();
        ~FirstApp();
        FirstApp(const FirstApp&) = delete;
        FirstApp& operator=(const FirstApp&) = delete;
        void run();
    private:
        void loadGameObjects();


        LveWindow Window{ WIDTH, HEIGHT, "Vulkan Window" };
        LveDevice lveDevice{ Window };
        LveRender lveRender{ Window, lveDevice };
        std::vector<LveGameObject> gameObjects;
    };
}
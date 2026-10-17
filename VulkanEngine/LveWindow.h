#pragma once

#define GLFW_INCLUDE_VULKAN
#include<string>
#include<GLFW/glfw3.h>
namespace lve 
{

	class LveWindow
	{
	public:
		LveWindow(int w,int h,std::string name);
		~LveWindow();

		LveWindow(const LveWindow &) = delete;
		LveWindow& operator=(const LveWindow&) = delete;

		bool ShouldClose() { return glfwWindowShouldClose(Window); };
		VkExtent2D getExtent() { return { static_cast<uint32_t> (width),static_cast<uint32_t>(height) }; };
		bool wasWindowResized() { return frameBufferResized; };
		void resetWindowResizedFlag() { frameBufferResized = false; };
		GLFWwindow* getGLFWwindow() const{ return Window; };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		int width;
		int height;
		bool frameBufferResized = false;

		std::string WindowName;
		GLFWwindow* Window;
		void InitWindow();

		static void frameBufferResizedCallback(GLFWwindow* window, int wideth, int height);
	};
}
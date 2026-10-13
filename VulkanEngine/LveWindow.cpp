#include "lveWindow.h"
#include<stdexcept>

namespace lve
{
	LveWindow::LveWindow(int w, int h, std::string name) :width(w), height(h), WindowName(name)
	{
		InitWindow();
	}

	LveWindow::~LveWindow()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();
	}

	void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, Window, nullptr, surface) != VK_SUCCESS)
		{
			throw::std::runtime_error("failed to create window surface");
		}
	}

	void LveWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		Window = glfwCreateWindow(width, height, WindowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(Window, this);
		glfwSetFramebufferSizeCallback(Window, frameBufferResizedCallback);

	}

	void LveWindow::frameBufferResizedCallback(GLFWwindow* window, int wideth, int height)
	{
		auto lveWindow = reinterpret_cast<LveWindow*>(glfwGetWindowUserPointer(window));
		lveWindow->frameBufferResized=true;
		lveWindow->width = wideth;
		lveWindow->height = height;
	}
}

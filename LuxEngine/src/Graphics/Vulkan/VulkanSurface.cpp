#pragma once



#define VK_USE_PLATFORM_WIN32_KHR

#include "VulkanSurface.h"


#include "Core/Application.h"
#include "Core/Window.h"

#include "VulkanGraphicsAPI.h"

#include "Core/Application.h"
#include "Utils/Debug.h"

#include <vulkan/vulkan.h>

namespace Lux
{

VulkanSurface::VulkanSurface()
{

	// Important: Window has to be created first
	CHECK(vkCreateWin32SurfaceKHR(VulkanGraphicsAPI::GetHandle(), static_cast<VkWin32SurfaceCreateInfoKHR*>(Window::Get().get_surface_handle()), nullptr, &m_Surface) != VK_SUCCESS, "Failed to create vulkan surface");

	CHECK(glfwCreateWindowSurface(VulkanGraphicsAPI::GetHandle(), static_cast<GLFWwindow*>(Window::GetHandle()), nullptr, &m_Surface) != VK_SUCCESS, "GLFW failed to connect surface")
}

VulkanSurface::~VulkanSurface()
{
	vkDestroySurfaceKHR(VulkanGraphicsAPI::GetHandle(), m_Surface, nullptr);
}

}
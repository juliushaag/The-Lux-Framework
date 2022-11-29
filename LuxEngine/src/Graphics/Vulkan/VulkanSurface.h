#pragma once

#include <vulkan/vulkan.h>


namespace Lux
{

class VulkanSurface
{
public:
	VulkanSurface();

	~VulkanSurface();

private:

	VkSurfaceKHR m_Surface{};
};

}
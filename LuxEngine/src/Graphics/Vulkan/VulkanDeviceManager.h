#pragma once

#include <vulkan/vulkan.h>

#include "Utils/Types.h"
#include "Utils/Debug.h"
#include "Utils/Console.h"

#include "VulkanDevice.h"

#include "Core/Application.h"

namespace Lux
{

class VulkanDeviceManager
{
public:

	VulkanDeviceManager();

	[[nodiscard]] inline VulkanDevice& get_highest_rated_device() const
	{
		CHECK(m_UsedDevice.second != nullptr, "No valid device present");
		return *m_UsedDevice.second;
	}

	~VulkanDeviceManager()
	{
		Console::Info("Destroying device manager");
	}

private:

	static inline u32 rate_device(VkPhysicalDevice device, VkPhysicalDeviceProperties& props, VkPhysicalDeviceFeatures& features, DeviceFamilyQueues& queues)
	{

		u32 rating =  (
				   props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
				   // or
				   props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
			   ) &&
			   // and
			   features.geometryShader &&
			   //and
			   queues.family_queues.has_value();

		rating += (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) * 50;

		rating += props.limits.maxImageDimension2D;

		return rating;
	}

	static inline DeviceFamilyQueues get_device_family_queues(VkPhysicalDevice device)
	{
		DeviceFamilyQueues indices;
		u32 queue_family_device_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_device_count, nullptr);

		List<VkQueueFamilyProperties> queueFamilies(queue_family_device_count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_device_count, queueFamilies.data());


		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.family_queues = i;
			}

			i++;
		}

		return indices;
	}

private:

	Pair<u32, VulkanDevice*> m_UsedDevice = { 0, nullptr };
	List<VulkanDevice> m_Devices;
};


}
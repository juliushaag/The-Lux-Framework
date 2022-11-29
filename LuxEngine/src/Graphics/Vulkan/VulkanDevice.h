#pragma once

#include <vulkan/vulkan.h>

#include "Utils/Types.h"


namespace Lux
{

struct DeviceFamilyQueues
{
	Optional<u32> family_queues;
};

class VulkanDevice
{
public:
	VulkanDevice(VkPhysicalDevice physical_device, VkPhysicalDeviceProperties& props, VkPhysicalDeviceFeatures& features, DeviceFamilyQueues queue, u32 rating)
		: 	m_PhysicalDevice(physical_device),
			m_Properties(props),
			m_Features(features),
			m_FamilyQueue(queue),
			m_Rating(rating)
	{
	}

	~VulkanDevice();


	[[nodiscard]] inline u32 get_rating() const
	{
		return m_Rating;
	}

	void select_device();


private:

	VkDevice m_Device;
	VkQueue m_GraphicsQueue;

	VkPhysicalDeviceProperties m_Properties;
	VkPhysicalDeviceFeatures m_Features;
	VkPhysicalDevice m_PhysicalDevice;

	DeviceFamilyQueues m_FamilyQueue;

	u32 m_Rating;

	bool m_LogicalDeviceInitialized = false;
};


}
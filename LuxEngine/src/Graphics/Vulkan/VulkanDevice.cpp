

#include "VulkanDevice.h"



#include "VulkanGraphicsAPI.h"

namespace Lux
{


void VulkanDevice::select_device()
{
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = m_FamilyQueue.family_queues.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &m_Features;

	createInfo.enabledExtensionCount = 0;

	if (VulkanGraphicsAPI::DebugEnabled()) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanGraphicsAPI::GetValidationLayers().size());
		createInfo.ppEnabledLayerNames = VulkanGraphicsAPI::GetValidationLayers().data();
	} else {
		createInfo.enabledLayerCount = 0;
	}

	Console::Info("Created logical device {}", (void*)&m_Device);
	CHECK(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) == VK_SUCCESS, "Failed to create logical device");


	m_LogicalDeviceInitialized = true;

	vkGetDeviceQueue(m_Device, m_FamilyQueue.family_queues.value(), 0, &m_GraphicsQueue);
}

VulkanDevice::~VulkanDevice()
{
	if(!m_LogicalDeviceInitialized)
		return;

	Console::Info("Destroyed logical device {}", (void*)&m_Device);
	vkDestroyDevice(m_Device, nullptr);
}
}
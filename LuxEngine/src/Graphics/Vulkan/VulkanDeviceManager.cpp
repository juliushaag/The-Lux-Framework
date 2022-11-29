#pragma once


#include "VulkanDeviceManager.h"

#include "VulkanGraphicsAPI.h"

namespace Lux
{

VulkanDeviceManager::VulkanDeviceManager()
{
	VkInstance instance = VulkanGraphicsAPI::GetHandle();

	u32 physical_device_count = 0;
	vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

	CHECK(physical_device_count != 0, "No rendering device found");
	m_Devices.reserve(physical_device_count);


	List<VkPhysicalDevice> devices(physical_device_count);
	vkEnumeratePhysicalDevices(instance, &physical_device_count, devices.data());

	VkPhysicalDeviceProperties props{};
	VkPhysicalDeviceFeatures features{};
	DeviceFamilyQueues queue{};


	for(auto device : devices)
	{
		vkGetPhysicalDeviceProperties(device, &props);
		vkGetPhysicalDeviceFeatures(device, &features);

		queue = get_device_family_queues(device);


		u32 device_rating = rate_device(device, props, features, queue);
		if(!device_rating)
		{
			Console::Warn("Physical device {} is not suitable for rendering", props.deviceName);
			continue;
		}


		Console::Info("Found physical device {} with id: {}", props.deviceName, props.deviceID);

		m_Devices.emplace_back(device,props, features, queue, device_rating);

		if(m_UsedDevice.first < device_rating)
			m_UsedDevice = { device_rating, &m_Devices.back() };
	}

	CHECK(m_UsedDevice.first > 0, "Failed to find a rendering device");
}

}
#pragma once

#include <vulkan/vulkan.h>
#include "Graphics/GraphicsAPI.h"

#include <glfw/glfw3.h>

#include "Utils/Debug.h"
#include "Utils/Console.h"

#include "Graphics/Graphics.h"
#include "VulkanDeviceManager.h"
#include "VulkanSurface.h"

namespace Lux
{

class VulkanGraphicsAPI : public GraphicsAPI
{
public:

	explicit VulkanGraphicsAPI(bool activate_debug = false)
		: 	m_DebugEnabled(activate_debug)
	{
		CHECK(s_Instance == nullptr, "Invalid amount of vulkan graphic layers have in created");
		s_Instance = this;

		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = "Vulkan Program",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "No Engine",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = VK_API_VERSION_1_0
		};


		load_extensions();
		load_available_layers();

		if(m_DebugEnabled)
			push_debug_layer();


		VkInstanceCreateInfo create_info {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast<u32>(m_ValidationLayers.size()),
			.ppEnabledLayerNames = m_ValidationLayers.data(),
			.enabledExtensionCount = static_cast<u32>(m_Extensions.size()),
			.ppEnabledExtensionNames = m_Extensions.data(),
		};


		VkDebugUtilsMessengerCreateInfoEXT debug_create_info{};
		if(m_DebugEnabled)
		{
			debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debug_create_info.pfnUserCallback = DebugCallback;
			debug_create_info.pUserData = this;

			create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;

		}


		VkResult result = vkCreateInstance(&create_info, nullptr, &m_Instance);

		switch(result) {

			case VK_SUCCESS:
				m_ErrorCode = GraphicsErrorCode::SUCCESS;
				break;
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				m_ErrorCode = GraphicsErrorCode::OUT_OF_HOST_MEMORY;
				break;
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				m_ErrorCode = GraphicsErrorCode::OUT_OF_DEVICE_MEMORY;
				break;
			case VK_ERROR_INITIALIZATION_FAILED:
				m_ErrorCode = GraphicsErrorCode::FAILED;
				break;
			case VK_ERROR_LAYER_NOT_PRESENT:
				m_ErrorCode = GraphicsErrorCode::LAYER_NOT_PRESENT;
				break;
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				m_ErrorCode = GraphicsErrorCode::EXTENSION_NOT_PRESENT;
				break;
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				m_ErrorCode = GraphicsErrorCode::INCOMPATIBLE_DRIVER;
				break;

			default:
				TODO();
		}

		if(m_DebugEnabled)
			init_debug_callback(debug_create_info);

		m_DeviceManager = create_scope<VulkanDeviceManager>();

		auto& m_LogicalDevice = m_DeviceManager->get_highest_rated_device();

		m_LogicalDevice.select_device();

		m_Surface = create_scope<VulkanSurface>();
	}


	~VulkanGraphicsAPI() override
	{
		m_Surface.reset(nullptr);
		m_DeviceManager.reset(nullptr);

		if(m_DebugEnabled)
			shutdown_debug_callback();

		Console::Info("Destroyed vulkan instance {}", (void*)m_Instance);
		vkDestroyInstance(m_Instance, nullptr);

		s_Instance = nullptr;
	}

public:

	static VulkanGraphicsAPI& Get()
	{
		return *s_Instance;
	}

	static const List<const char*>& GetValidationLayers()
	{
		return Get().m_ValidationLayers;
	}

	static bool DebugEnabled()
	{
		return Get().m_DebugEnabled;
	}

	static List<const char *> GetValidationLayer()
	{
		return Get().m_ValidationLayers;
	}

	static VkInstance GetHandle()
	{
		return Get().m_Instance;
	}

private:

	inline void load_available_layers()
	{
		u32 layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

		m_AvailableLayers.resize(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, (VkLayerProperties *) m_AvailableLayers.data());
	}

	inline void push_debug_layer()
	{
		for(auto layer : m_DebugLayers)
		{
			bool found_layer = false;
			for(auto& available_layer : m_AvailableLayers)
			{
				if(strcmp(layer, available_layer.layerName) == 0)
				{
					found_layer = true;
					break;
				}
			}

			if(!found_layer)
			{
				Console::Error("Failed to load debug layer {}", layer);
				continue;
			}

			m_ValidationLayers.emplace_back(layer);
		}
	}



	void init_debug_callback(const VkDebugUtilsMessengerCreateInfoEXT& create_info)
	{
		if (CreateDebugUtilMessengerExt(m_Instance, &create_info, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
			Console::Error("VK Failed to set up debug messenger!");
		}
	}

	void shutdown_debug_callback()
	{
		DestroyDebugUtilMessengerExt(m_Instance, m_DebugMessenger, nullptr);
	}

	inline void load_extensions()
	{

		u32 glfw_extension_count = 0;
		const char** glfw_extensions;
		glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

		m_Extensions = List<const char*>(glfw_extensions, glfw_extensions + glfw_extension_count);

		if (m_DebugEnabled) {
			m_Extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

	}



	// Vulkan debug callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data)
		{

		if(message_severity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			return VK_FALSE;

		Console::Error("Vulkan: {}", callback_data->pMessage);
		return VK_TRUE;
	}

	static inline VkResult CreateDebugUtilMessengerExt
	(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		} else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}
	static inline void DestroyDebugUtilMessengerExt
	(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debug_messenger, pAllocator);
		}
	}

private:

	static VulkanGraphicsAPI* s_Instance;

	const bool m_DebugEnabled;


	Scope<VulkanDeviceManager> m_DeviceManager{};
	Scope<VulkanSurface> m_Surface{};

	const Array<const char*, 1> m_DebugLayers {
		"VK_LAYER_KHRONOS_validation"
	};

	List<const char*> m_ValidationLayers{};
	List<VkLayerProperties> m_AvailableLayers{};

	List<const char*> m_Extensions{};

	VkDebugUtilsMessengerEXT m_DebugMessenger{};
	VkInstance m_Instance{};
};

}
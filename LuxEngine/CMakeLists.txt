cmake_minimum_required(VERSION 3.23)

project(LuxEngine)

set(SRC_FILES src/Core/Application.cpp src/Core/Window.cpp src/Core/Libraries.cpp src/Graphics/GraphicsAPI.cpp src/Graphics/GraphicsManager.cpp src/Graphics/Vulkan/VulkanDevice.cpp src/Graphics/Vulkan/VulkanGraphicsAPI.cpp src/Graphics/Vulkan/VulkanSurface.cpp src/Graphics/Vulkan/VulkanDeviceManager.cpp)

set(HEADER_FILES src/Core/Application.h src/Core/Window.h src/Utils/Types.h src/Core/Libraries.h src/Utils/Debug.h src/Core/KeyCodes.h src/Graphics/GraphicsAPI.h src/Graphics/Vulkan/VulkanGraphicsAPI.h src/Graphics/GraphicsManager.h src/Graphics/Graphics.h src/Utils/Console.h src/Graphics/Vulkan/VulkanDeviceManager.h src/Graphics/Vulkan/VulkanDevice.h src/Graphics/Vulkan/VulkanSurface.h)




add_subdirectory(lib)

set(LUX_LIBRARIES glfw glm glad Vulkan::Vulkan fmt)
set(LUX_LIB_INCLUDES lib/glfw/include/ lib/glm/glm/ lib/glad/include/ lib/vulkan/include ${Vulkan_INCLUDE_DIR} lib/fmt/include)

add_library(Lux STATIC ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(Lux PRIVATE ${LUX_LIBRARIES})


target_include_directories(Lux SYSTEM PUBLIC src/ ${LUX_LIB_INCLUDES})
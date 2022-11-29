#pragma once

#include "GraphicsAPI.h"

#include "Vulkan/VulkanGraphicsAPI.h"

namespace Lux
{


Scope<GraphicsAPI> GraphicsAPI::Create(bool debug)
{
	return create_scope<VulkanGraphicsAPI>(debug);
}
}
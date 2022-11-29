#pragma once

#include "GraphicsManager.h"

#include "Utils/Debug.h"
#include "Utils/Console.h"

namespace Lux
{

GraphicsAPIType GraphicsManager::s_Type = GraphicsAPIType::UNKNOWN;
Scope<GraphicsAPI> GraphicsManager::s_API = nullptr;

bool GraphicsManager::Init(GraphicsAPIType type, bool debug)
{
	Verify(type != GraphicsAPIType::UNKNOWN);
	s_Type = type;

	switch(s_Type) {
		case GraphicsAPIType::VULKAN:
			s_API = GraphicsAPI::Create(debug);
			break;

		case GraphicsAPIType::UNKNOWN:
			TODO();
			break;
	}

	GraphicsErrorCode error_code = s_API->get_error_code();

	switch(error_code)
	{
		case GraphicsErrorCode::SUCCESS:
			return true;

		case GraphicsErrorCode::OUT_OF_HOST_MEMORY:
			Console::Error("Host out of memory");
			break;
		case GraphicsErrorCode::OUT_OF_DEVICE_MEMORY:
			Console::Error("Graphics device out of memory");
			break;
		case GraphicsErrorCode::FAILED:
			Console::Error("Failed to initialize graphics api");
			break;
		case GraphicsErrorCode::LAYER_NOT_PRESENT:
			Console::Error("Graphics validation layer not present");
			break;
		case GraphicsErrorCode::EXTENSION_NOT_PRESENT:
			Console::Error("Graphics extension not present");
			break;
		case GraphicsErrorCode::INCOMPATIBLE_DRIVER:
			Console::Error("Graphics driver is not compatible");
			break;
	}

	return false;

}

void GraphicsManager::Shutdown()
{
	s_API.reset();
	s_Type = GraphicsAPIType::UNKNOWN;
}


}
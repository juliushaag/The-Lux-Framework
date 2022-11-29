#pragma once

#include "Utils/Types.h"

#include "GraphicsAPI.h"

namespace Lux
{

enum class GraphicsAPIType
{
	UNKNOWN,
	VULKAN
};


class GraphicsManager
{
public:

	static bool Init(GraphicsAPIType type, bool debug);

	static void Shutdown();

	static GraphicsAPIType GetAPI()
	{
		return s_Type;
	}

private:

	static GraphicsAPIType s_Type;

	static Scope<GraphicsAPI> s_API;

};


}
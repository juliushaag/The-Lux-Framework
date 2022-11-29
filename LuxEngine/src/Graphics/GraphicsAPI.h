#pragma once
#include "Utils/Types.h"

#include "Graphics.h"

namespace Lux
{

class GraphicsAPI
{
public:

	virtual ~GraphicsAPI() = default;

	static Scope<GraphicsAPI> Create(bool debug);

	GraphicsErrorCode get_error_code()
	{
		return m_ErrorCode;
	}

protected:

	GraphicsErrorCode m_ErrorCode = GraphicsErrorCode::FAILED;

	GraphicsAPI() = default;

};


}
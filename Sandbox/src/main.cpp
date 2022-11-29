#include <iostream>

#include "Core/Application.h"

#include "Core/Libraries.h"

using namespace Lux;

int main()
{
	if(!LoadLibraries())
		return 1;

	{
		Lux::Application app;

		app.loop();
	}

	UnloadLibraries();

	return 0;
}
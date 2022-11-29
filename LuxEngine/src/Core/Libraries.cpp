#include "Application.h"
#include "Libraries.h"


#include <GLFW/glfw3.h>


namespace Lux
{

static bool is_loaded = false;

// Defined in Libraries.cpp for better library enclosure
bool LoadLibraries()
{
	if(!glfwInit())
		return false;


	is_loaded = true;
	return is_loaded;
}

void UnloadLibraries()
{
	glfwTerminate();

	is_loaded = false;
}

bool Lux::LibrariesLoaded() {
	return is_loaded;
}


}



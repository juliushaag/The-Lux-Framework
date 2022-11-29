#include "Core/Application.h"

#include "Libraries.h"

#include "Utils/Console.h"
#include "Graphics/GraphicsManager.h"

namespace Lux
{

Application* Application::s_Instance = nullptr;

Application::Application()
{
	Verify(s_Instance == nullptr);

	s_Instance = this;

	Console::Init();

	m_Window = create_scope<Window>(*this, 1280, 720, "Test");

	CHECK(LibrariesLoaded(), "LoadLibraries() needs to be called before constructing an application class")

	CHECK(GraphicsManager::Init(GraphicsAPIType::VULKAN, true), "Failed to start the graphics manager");

	m_Window->show();

}

void Application::loop()
{

	while (m_Running)
	{
		Window::PollEvents();
	}
}



Window& Application::get_window()
{
	return *m_Window;
}

void Application::terminate() {
	m_Running = false;
}

void Application::on_close()
{
	m_Running = false;
}

void Application::on_key_action(KeyCode key, KeyCode mod, KeyState state)
{

}

void Application::on_scroll(v2 scroll_distance)
{

}

void Application::on_resize(v2 size)
{

}

void Application::on_mouse_move(v2 position)
{

}

void Application::on_maximize(bool is_maximized)
{

}

void Application::on_focus(bool is_focused)
{

}


}
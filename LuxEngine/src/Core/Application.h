#pragma once

#include <string>

#include "Window.h"

#include "KeyCodes.h"

namespace Lux
{

class Application
{
public:

	Application();

    void loop();

    void terminate();

	Window& get_window();

public:

	void on_close();

	void on_key_action(KeyCode key, KeyCode mod, KeyState state);

	void on_scroll(v2 scroll_distance);

	void on_resize(v2 size);

	void on_mouse_move(v2 position);

	void on_maximize(bool is_maximized);

	void on_focus(bool is_focused);

public:

	static Application* Get()
	{
		return s_Instance;
	}


	static void Crash()
	{
		Verify(s_Instance);
		s_Instance->m_Running = false;
		exit(1);
	}

private:

    Scope<Window> m_Window = nullptr;

    bool m_Running = true;


	static Application* s_Instance;

};

}
#pragma once

#include "Utils/Types.h"
#include "Utils/Debug.h"



namespace Lux
{
class Application;

class Window
{
public:

	Window(Application& app, int width, int height, const std::string& title);

	~Window();

  	void set_title(const String& title);
	  
	void set_fullscreen();
	
	void enable_vsync();
	
	void disable_vsync();
	
	[[nodiscard]] bool is_initialized() const;

	void set_size(int width, int height);

	void show();

	void hide();

	inline int get_width() const
	{
		return m_Width;
	}

	inline int get_height() const
	{
		return m_Height;
	}

	void* get_surface_handle();


public:

	static void* GetHandle()
	{
		return s_Instance->m_WindowHandle;
	}

	static Window& Get()
	{
		return *s_Instance;
	}

	static void PollEvents();

private:

	static Window* s_Instance;

	Application& m_App;

	int m_Width, m_Height;

	bool m_Initialized;
	
	void* m_WindowHandle;

	void* m_VkSurfaceInfo;

};


}
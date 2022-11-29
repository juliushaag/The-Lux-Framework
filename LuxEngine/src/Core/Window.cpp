#include "Window.h"

#include "Utils/Console.h"

#include "KeyCodes.h"

#include "Application.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>
#include <GLFW/glfw3native.h>

#include "KeyCodes.h"

namespace Lux {



static constexpr KeyState glfw_to_lux_keystate[] = {
	KeyState::RELEASED,
	KeyState::PRESSED,
	KeyState::REPEATED
};


[[maybe_unused]] static inline MouseKey glfw_to_lux_mousekey(GLenum key);

static inline KeyCode glfw_to_lux_keycode(GLenum keycode);

Window* Window::s_Instance = nullptr;

Window::Window(Application& app, int width, int height, const std::string &title)
	:	m_App(app)
{
	CHECK(s_Instance == nullptr, "Only one main window can be active at a time")

	s_Instance = this;

	// Setting window creation hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (!window) {
		m_Initialized = false;
		return;
	}

	m_Width = width;

	m_Height = height;

	glfwMakeContextCurrent(window);

	// Storing a pointer to this class ;) for event handling
	glfwSetWindowUserPointer(window, this);

	// Calling the appropriate window callback functions
	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_close();
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mod)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_key_action(glfw_to_lux_keycode(key), glfw_to_lux_keycode(mod), glfw_to_lux_keystate[action]);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double scroll_x, double scroll_y)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_scroll({scroll_x, scroll_y});
	});

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_resize({width, height});
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double pos_x, double pos_y)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_mouse_move({pos_x, pos_y});
	});

	glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int is_maximized)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_maximize(static_cast<bool>(is_maximized));
	});

	glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
	{
		auto window_obj = static_cast<decltype(this)>(glfwGetWindowUserPointer(window));

		window_obj->m_App.on_focus(static_cast<bool>(focused));
	});

	m_WindowHandle = static_cast<void *>(window);
	m_Initialized = true;

	auto vk_surface_info = new VkWin32SurfaceCreateInfoKHR{};
	vk_surface_info->sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	vk_surface_info->hwnd = glfwGetWin32Window(static_cast<GLFWwindow*>(Window::GetHandle()));
	vk_surface_info->hinstance = GetModuleHandle(nullptr);

	m_VkSurfaceInfo = static_cast<void*>(vk_surface_info);
}

void* Window::get_surface_handle()
{
	return m_VkSurfaceInfo;
}


void Window::set_title(const String &title) {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwSetWindowTitle(window, title.c_str());
}

void Window::set_fullscreen() {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwMaximizeWindow(window);
}

void Window::enable_vsync() {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwSwapInterval(1);
}

void Window::disable_vsync() {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwSwapInterval(0);
}

bool Window::is_initialized() const {
	return m_Initialized;
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::set_size(int width, int height) {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwSetWindowSize(window, width, height);
}

void Window::show() {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwShowWindow(window);
}

void Window::hide() {
	auto *window = static_cast<GLFWwindow *>(m_WindowHandle);

	glfwHideWindow(window);
}


Window::~Window() {

	delete static_cast<VkWin32SurfaceCreateInfoKHR*>(m_VkSurfaceInfo);
	glfwDestroyWindow(static_cast<GLFWwindow *>(m_WindowHandle));
}

KeyCode glfw_to_lux_keycode(GLenum keycode)
{
	switch(keycode)
	{
		case static_cast<GLenum>(GLFW_KEY_UNKNOWN) : 	return KeyCode::UNKNOWN;
		case GLFW_KEY_SPACE : 		return KeyCode::SPACE;
		case GLFW_KEY_APOSTROPHE : 	return KeyCode::APOSTROPHE;
		case GLFW_KEY_COMMA : 		return KeyCode::COMMA;
		case GLFW_KEY_MINUS : 		return KeyCode::MINUS;
		case GLFW_KEY_PERIOD : 		return KeyCode::PERIOD;
		case GLFW_KEY_SLASH : 		return KeyCode::SLASH;
		case GLFW_KEY_SEMICOLON : 	return KeyCode::SEMICOLON;
		case GLFW_KEY_EQUAL : 		return KeyCode::EQUAL;
		case GLFW_KEY_A : return KeyCode::A;
		case GLFW_KEY_B : return KeyCode::B;
		case GLFW_KEY_C : return KeyCode::C;
		case GLFW_KEY_D : return KeyCode::D;
		case GLFW_KEY_E : return KeyCode::E;
		case GLFW_KEY_F : return KeyCode::F;
		case GLFW_KEY_G : return KeyCode::G;
		case GLFW_KEY_H : return KeyCode::H;
		case GLFW_KEY_I : return KeyCode::I;
		case GLFW_KEY_J : return KeyCode::J;
		case GLFW_KEY_K : return KeyCode::K;
		case GLFW_KEY_L : return KeyCode::L;
		case GLFW_KEY_M : return KeyCode::M;
		case GLFW_KEY_N : return KeyCode::N;
		case GLFW_KEY_O : return KeyCode::O;
		case GLFW_KEY_P : return KeyCode::P;
		case GLFW_KEY_Q : return KeyCode::Q;
		case GLFW_KEY_R : return KeyCode::R;
		case GLFW_KEY_S : return KeyCode::S;
		case GLFW_KEY_T : return KeyCode::T;
		case GLFW_KEY_U : return KeyCode::U;
		case GLFW_KEY_V : return KeyCode::V;
		case GLFW_KEY_W : return KeyCode::W;
		case GLFW_KEY_X : return KeyCode::X;
		case GLFW_KEY_Y : return KeyCode::Y;
		case GLFW_KEY_Z : return KeyCode::Z;
		case GLFW_KEY_LEFT_BRACKET : 	return KeyCode::LEFT_BRACKET;
		case GLFW_KEY_BACKSLASH : 		return KeyCode::BACKSLASH;
		case GLFW_KEY_RIGHT_BRACKET : 	return KeyCode::RIGHT_BRACKET;
		case GLFW_KEY_GRAVE_ACCENT : 	return KeyCode::GRAVE_ACCENT;
		case GLFW_KEY_WORLD_1 : 		return KeyCode::WORLD_1;
		case GLFW_KEY_WORLD_2 : 		return KeyCode::WORLD_2;
		case GLFW_KEY_ESCAPE : 			return KeyCode::ESCAPE;
		case GLFW_KEY_ENTER : 			return KeyCode::ENTER;
		case GLFW_KEY_TAB : 			return KeyCode::TAB;
		case GLFW_KEY_BACKSPACE : 		return KeyCode::BACKSPACE;
		case GLFW_KEY_INSERT : 			return KeyCode::INSERT;
		case GLFW_KEY_RIGHT : 			return KeyCode::RIGHT;
		case GLFW_KEY_LEFT : 			return KeyCode::LEFT;
		case GLFW_KEY_DOWN : 			return KeyCode::DOWN;
		case GLFW_KEY_UP : 				return KeyCode::UP;
		case GLFW_KEY_PAGE_UP : 		return KeyCode::PAGE_UP;
		case GLFW_KEY_PAGE_DOWN : 		return KeyCode::PAGE_DOWN;
		case GLFW_KEY_HOME : 			return KeyCode::HOME;
		case GLFW_KEY_END : 			return KeyCode::END;
		case GLFW_KEY_CAPS_LOCK : 		return KeyCode::CAPS_LOCK;
		case GLFW_KEY_SCROLL_LOCK : 	return KeyCode::SCROLL_LOCK;
		case GLFW_KEY_NUM_LOCK : 		return KeyCode::NUM_LOCK;
		case GLFW_KEY_PRINT_SCREEN : 	return KeyCode::PRINT_SCREEN;
		case GLFW_KEY_PAUSE : 			return KeyCode::PAUSE;
		case GLFW_KEY_F1 : return KeyCode::F1;
		case GLFW_KEY_F2 : return KeyCode::F2;
		case GLFW_KEY_F3 : return KeyCode::F3;
		case GLFW_KEY_F4 : return KeyCode::F4;
		case GLFW_KEY_F5 : return KeyCode::F5;
		case GLFW_KEY_F6 : return KeyCode::F6;
		case GLFW_KEY_F7 : return KeyCode::F7;
		case GLFW_KEY_F8 : return KeyCode::F8;
		case GLFW_KEY_F9 : return KeyCode::F9;
		case GLFW_KEY_F10 : return KeyCode::F10;
		case GLFW_KEY_F11 : return KeyCode::F11;
		case GLFW_KEY_F12 : return KeyCode::F12;
		case GLFW_KEY_F13 : return KeyCode::F13;
		case GLFW_KEY_F14 : return KeyCode::F14;
		case GLFW_KEY_F15 : return KeyCode::F15;
		case GLFW_KEY_F16 : return KeyCode::F16;
		case GLFW_KEY_F17 : return KeyCode::F17;
		case GLFW_KEY_F18 : return KeyCode::F18;
		case GLFW_KEY_F19 : return KeyCode::F19;
		case GLFW_KEY_F20 : return KeyCode::F20;
		case GLFW_KEY_F21 : return KeyCode::F21;
		case GLFW_KEY_F22 : return KeyCode::F22;
		case GLFW_KEY_F23 : return KeyCode::F23;
		case GLFW_KEY_F24 : return KeyCode::F24;
		case GLFW_KEY_F25 : return KeyCode::F25;
		case GLFW_KEY_0 :                   return KeyCode::ZERO;
		case GLFW_KEY_1 :                   return KeyCode::ONE;
		case GLFW_KEY_2 :                   return KeyCode::TWO;
		case GLFW_KEY_3 :                   return KeyCode::THREE;
		case GLFW_KEY_4 :                   return KeyCode::FOUR;
		case GLFW_KEY_5 :                   return KeyCode::FIVE;
		case GLFW_KEY_6 :                   return KeyCode::SIX;
		case GLFW_KEY_7 :                   return KeyCode::SEVEN;
		case GLFW_KEY_8 :                   return KeyCode::EIGHT;
		case GLFW_KEY_9 :                   return KeyCode::NINE;
		case GLFW_KEY_KP_DECIMAL :          return KeyCode::KP_DECIMAL;
		case GLFW_KEY_KP_DIVIDE :           return KeyCode::KP_DIVIDE;
		case GLFW_KEY_KP_MULTIPLY :         return KeyCode::KP_MULTIPLY;
		case GLFW_KEY_KP_SUBTRACT :         return KeyCode::KP_SUBTRACT;
		case GLFW_KEY_KP_ADD :              return KeyCode::KP_ADD;
		case GLFW_KEY_KP_ENTER :            return KeyCode::KP_ENTER;
		case GLFW_KEY_KP_EQUAL :            return KeyCode::KP_EQUAL;
		case GLFW_KEY_LEFT_SHIFT :          return KeyCode::LEFT_SHIFT;
		case GLFW_KEY_LEFT_CONTROL :        return KeyCode::LEFT_CONTROL;
		case GLFW_KEY_LEFT_ALT :            return KeyCode::LEFT_ALT;
		case GLFW_KEY_LEFT_SUPER :          return KeyCode::LEFT_SUPER;
		case GLFW_KEY_RIGHT_SHIFT :         return KeyCode::RIGHT_SHIFT;
		case GLFW_KEY_RIGHT_CONTROL :       return KeyCode::RIGHT_CONTROL;
		case GLFW_KEY_RIGHT_ALT :           return KeyCode::RIGHT_ALT;
		case GLFW_KEY_RIGHT_SUPER :         return KeyCode::RIGHT_SUPER;
		case GLFW_KEY_MENU :                return KeyCode::MENU;
	}

	return KeyCode::UNKNOWN;
}

[[maybe_unused]] MouseKey glfw_to_lux_mousekey(GLenum key)
{
	switch(key)
	{
		case GLFW_MOUSE_BUTTON_1: return MouseKey::LEFT;
		case GLFW_MOUSE_BUTTON_2: return MouseKey::RIGHT;
		case GLFW_MOUSE_BUTTON_3: return MouseKey::MIDDLE;
		case GLFW_MOUSE_BUTTON_4: return MouseKey::FOUR;
		case GLFW_MOUSE_BUTTON_5: return MouseKey::FIVE;
		case GLFW_MOUSE_BUTTON_6: return MouseKey::SIX;
		case GLFW_MOUSE_BUTTON_7: return MouseKey::SEVEN;
		case GLFW_MOUSE_BUTTON_8: return MouseKey::EIGHT;
	};
	return MouseKey::UNKNOWN;
}

}
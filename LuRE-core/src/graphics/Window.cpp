#include <iostream>

#include "Window.h"

namespace lumi {
	namespace graphics {

		Window::Window(const char *name, int width, int height) 
			:m_mouseOldPosition(width / 2.0f, height / 2.0f)
		{
			m_name = name;
			m_width = width;
			m_height = height;
			m_isFirstPosition = true;

			if (!init()) {
				glfwTerminate();
			}

			for (int i = 0; i < MAX_KEYS; i++) {
				m_keys[i] = false;
				m_keyState[i] = false;
				m_keyTyped[i] = false;
			}

			for (int i = 0; i < MAX_BUTTONS; i++) {
				m_mouseButtons[i] = false;
				m_mouseState[i] = false;
				m_mouseClicked[i] = false;
			}

		}

		Window::~Window()
		{
			glfwTerminate();
		}

		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_window) == 1;
		}

		bool Window::init()
		{
			if (!glfwInit()) {
				std::cout << "Failed to initialize glfw!" << std::endl;
				return false;
			}

			m_window = glfwCreateWindow(m_width, m_height, m_name, nullptr, nullptr);

			if (!m_window) {
				std::cout << "Failed to create a window!" << std::endl;
				return false;
			}

			glfwMakeContextCurrent(m_window);
			glfwSetWindowUserPointer(m_window, this);
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetFramebufferSizeCallback(m_window, window_resize);
			glfwSetKeyCallback(m_window, key_callback);
			glfwSetMouseButtonCallback(m_window, mouse_button_callback);
			glfwSetCursorPosCallback(m_window, cursor_position_callback);
			glfwSwapInterval(0);

			//Only AFTER context has been initialized!
			if (glewInit() != GLEW_OK) {

				std::cout << "Could not initialize GLEW!" << std::endl;
				return false;
			}

			std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

			return true;
		}


		void Window::clear() const 
		{
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::update()
		{
			GLenum error = glGetError();

			if (error != GL_NO_ERROR)
				std::cerr << "OpenGL Error: " << error << std::endl;
			
			glfwSwapBuffers(m_window);
			glfwPollEvents();			

		}

		void Window::updateInput()
		{
			for (int i = 0; i < MAX_KEYS; i++)
				m_keyTyped[i] = m_keys[i] && !m_keyState[i];


			for (int i = 0; i < MAX_BUTTONS; i++)
				m_mouseClicked[i] = m_mouseButtons[i] && !m_mouseState[i];


			memcpy(m_keyState, m_keys, MAX_KEYS);
			memcpy(m_mouseState, m_mouseButtons, MAX_BUTTONS);

		}

		void window_resize(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_width = width;
			win->m_height = height;
		}

		void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
		{
			Window* win = (Window*) glfwGetWindowUserPointer(window);
			win->m_keys[key] = action != GLFW_RELEASE;
		}

		void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) 
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			win->m_mouseButtons[button] = action != GLFW_RELEASE;
		}

		void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
		{
			Window* win = (Window*)glfwGetWindowUserPointer(window);

			win->m_mouseOldPosition = win->m_mousePosition;

			win->m_mousePosition.x = (float)xpos;
			win->m_mousePosition.y = (float)ypos;
		}

		bool Window::is_key_pressed(unsigned int keycode) const
		{

			if (keycode > MAX_KEYS)
				return false;

			return m_keys[keycode];
		}

		bool Window::is_key_typed(unsigned int keycode) const
		{

			if (keycode > MAX_KEYS)
				return false;

			return m_keyTyped[keycode];
		
		}

		bool Window::is_mouse_button_pressed(unsigned int button) const
		{

			if (button > MAX_BUTTONS)
				return false;

			return m_mouseButtons[button];
		}


		bool Window::is_mouse_button_clicked(unsigned int button) const
		{

			if (button > MAX_BUTTONS)
				return false;

			return m_mouseClicked[button];
		}


		const maths::vec2& Window::get_mouse_position() const
		{
			return m_mousePosition;
		}

		void Window::getMouseDisplacement(maths::vec2& displacement)
		{
			float dx = m_mousePosition.x - m_mouseOldPosition.x;
			float dy = m_mouseOldPosition.y - m_mousePosition.y;

			m_mouseOldPosition = m_mousePosition;

			displacement.x = dx;
			displacement.y = dy;
		}

	}
}



#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "..\maths\vec2.h"

namespace lumi {
	namespace graphics {

#define MAX_KEYS	1024
#define MAX_BUTTONS 32

		class Window {

		private:
			
			int m_height, m_width;
			const char *m_name;
			bool m_closed;
			bool m_isFirstPosition;
			
			GLFWwindow *m_window;

			bool m_mouseButtons[MAX_BUTTONS];
			bool m_mouseState[MAX_BUTTONS];
			bool m_mouseClicked[MAX_BUTTONS];
			maths::vec2 m_mousePosition;
			maths::vec2 m_mouseOldPosition;

			bool m_keys[MAX_KEYS];
			bool m_keyState[MAX_KEYS];
			bool m_keyTyped[MAX_KEYS];

		public:
			Window(const char*, int, int);
			~Window();
			bool closed() const;
			void update();
			void updateInput();
			void clear() const;

			inline int getWidth() const { return m_width; };
			inline int getHeight() const { return m_height; };

			bool is_key_pressed(unsigned int keycode) const;
			bool is_key_typed(unsigned int keycode) const;

			bool is_mouse_button_pressed(unsigned int button) const;
			bool is_mouse_button_clicked(unsigned int button) const;
			const maths::vec2& Window::get_mouse_position() const;
			void getMouseDisplacement(maths::vec2& displacement);
		private:

			bool init();

			friend void window_resize(GLFWwindow *window, int width, int height);
			friend void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
			friend void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
			friend void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
		};

	}
}

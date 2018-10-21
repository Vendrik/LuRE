#pragma once

#include <iostream>
#include <signal.h>
#include <GL\glew.h>

namespace lumi {

	#ifdef _DEBUG

	#define GlCall(x) \
			do { \
				char* c = nullptr, *file = __FILE__; \
				for(; *file!='\0'; file++) \
					if (*file == '\\' || *file=='/') \
						c = file; \
				if (*c == '\\') \
					c++; \
				x; \
				GLenum error = glGetError(); \
				if (error != GL_NO_ERROR){ \
					std::cerr << "OpenGL Error: " << error << " @" << c << ":" << __LINE__ << std::endl; \
					__debugbreak(); \
				} \
			} while (false)

	#else

		#define GlCall(x) x

	#endif

}
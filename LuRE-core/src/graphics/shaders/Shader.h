#pragma once

#include<GL\glew.h>
#include "..\..\maths\maths.h"

#include <unordered_map>

namespace lumi {
	namespace graphics {

		class Shader 
		{
		private:
			GLuint m_shaderId;
			const char* m_name;
			const char* m_vertPath;
			const char* m_fragPath;
			const char* m_vertSrc;
			const char* m_fragSrc;

			std::unordered_map<std::string, GLint> m_uniformMap;

		public:
			Shader(const char* vertPath, const char* fragPath);
			Shader(const char* name, const char* vertSrc, const char* fragSrc);
			~Shader();

			void setUniform1f(const GLchar* name, float value);
			void setUniform1fv(const GLchar* name, float* value, int count);
			void setUniform1i(const GLchar* name, int value);
			void setUniform2i(const GLchar* name, int* value);
			void setUniform3i(const GLchar* name, int* value);
			void setUniform4i(const GLchar* name, int* value);
			void setUniform1iv(const GLchar* name, int* value, int count);
			void setUniform2f(const GLchar* name, const maths::vec2& vector);
			void setUniform2f(const GLchar * name, float* vector);
			void setUniform3f(const GLchar* name, const maths::vec3& vector);
			void setUniform4f(const GLchar* name, const maths::vec4& vector);
			void setUniformMat4(const GLchar* name, const maths::mat4& matrix);

			void enable() const;
			void disable() const;

			static Shader* FromFile(const char* vertPath, const char* fragPath);
			static Shader* FromSource(const char* vertSrc, const char* fragSrc);
			static Shader* FromSource(const char* name, const char* vertSrc, const char* fragSrc);


		private:

			GLuint load(const char* vertSrc, const char* fragSrc);
			GLint getUniformLocation(const GLchar* name);

		};
	}
}
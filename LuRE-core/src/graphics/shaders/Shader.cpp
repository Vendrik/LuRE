#include "Shader.h"

#include "..\..\utils\file_utils.h"
#include "..\..\utils\Log.h"
#include "..\..\utils\Debug.h"

#include <iostream>
#include <string>
#include <vector>

namespace lumi {
	namespace graphics {

		Shader::Shader(const char * vertPath, const char * fragPath) : 
			m_name("Default Shaders"), m_vertPath(vertPath), m_fragPath(fragPath)
		{
			GlCall(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_maxUniformBlockBindings));

			std::string vertexStringSource = read_file(m_vertPath);
			std::string fragmentStringSource = read_file(m_fragPath);

			m_vertSrc = vertexStringSource.c_str();
			m_fragSrc = fragmentStringSource.c_str();

			m_shaderId = load(m_vertSrc, m_fragSrc);
		}

		Shader::Shader(const char * name, const char * vertSrc, const char * fragSrc):
			m_name(name), m_vertSrc(vertSrc), m_fragSrc(fragSrc)
		{
			m_shaderId = load(m_vertSrc, m_fragSrc);
		}

		Shader::~Shader()
		{
			GlCall(glDeleteProgram(m_shaderId));
		}

		void Shader::setUniform1f(const GLchar * name, float value)
		{
			GlCall(glUniform1f(getUniformLocation(name), value));
		}

		void Shader::setUniform1fv(const GLchar * name, float * value, int count)
		{
			GlCall(glUniform1fv(getUniformLocation(name), count, value));
		}

		void Shader::setUniform1i(const GLchar * name, int value)
		{
			GlCall(glUniform1i(getUniformLocation(name), value));
		}

		void Shader::setUniform2i(const GLchar * name, int* value)
		{
			GlCall(glUniform2i(getUniformLocation(name), value[0], value[1]));
		}

		void Shader::setUniform3i(const GLchar * name, int* value)
		{
			GlCall(glUniform3i(getUniformLocation(name), value[0], value[1], value[2]));
		}

		void Shader::setUniform4i(const GLchar * name, int* value)
		{
			GlCall(glUniform4i(getUniformLocation(name), value[0], value[1], value[2], value[3]));
		}

		void Shader::setUniform1iv(const GLchar * name, int * value, int count)
		{
			GlCall(glUniform1iv(getUniformLocation(name), count, value));
		}

		void Shader::setUniform2f(const GLchar * name, float* vector)
		{
			GlCall(glUniform2f(getUniformLocation(name), vector[0], vector[1]));
		}

		void Shader::setUniform2f(const GLchar * name, const maths::vec2 & vector)
		{
			GlCall(glUniform2f(getUniformLocation(name), vector.x, vector.y));
		}

		void Shader::setUniform3f(const GLchar * name, const maths::vec3 & vector)
		{
			GlCall(glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z));
		}

		void Shader::setUniform4f(const GLchar * name, const maths::vec4 & vector)
		{
			GlCall(glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w));
		}

		void Shader::setUniformMat4(const GLchar * name, const maths::mat4 & matrix)
		{
			GlCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements));
		}

		void Shader::setUniformBlockBinding(const GLchar * name, unsigned int binding_point)
		{
			if (binding_point >= m_maxUniformBlockBindings)
				LUMI_FATAL("Bind overflow, maximum allowed: %d, binding: %d", m_maxUniformBlockBindings, binding_point);

			GlCall(glUniformBlockBinding(m_shaderId, getUniformLocation(name, true), binding_point));
		}

		void Shader::enable() const
		{
			GlCall(glUseProgram(m_shaderId));
		}

		void Shader::disable() const
		{
			GlCall(glUseProgram(0));
		}

		Shader * Shader::FromFile(const char * vertPath, const char * fragPath)
		{
			return new Shader(vertPath, fragPath);
		}

		Shader * Shader::FromSource(const char * vertSrc, const char * fragSrc)
		{
			return new Shader("Default Shader", vertSrc, fragSrc);
		}

		Shader * Shader::FromSource(const char * name, const char * vertSrc, const char * fragSrc)
		{
			return new Shader(name, vertSrc, fragSrc);
		}

		GLuint Shader::load(const char* vertSrc, const char* fragSrc)
		{
			GLuint program = 0;  
			GLuint vertex = 0;   
			GLuint fragment = 0; 

			GlCall(program = glCreateProgram());
			GlCall(vertex = glCreateShader(GL_VERTEX_SHADER));
			GlCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
			GlCall(glShaderSource(vertex, 1, &vertSrc, nullptr));
			GlCall(glCompileShader(vertex));

			GLint res;
			GlCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &res));

			if (res == GL_FALSE)
			{
				GLint lenght;

				GlCall(glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &lenght));
				std::vector<char> error(lenght);
				GlCall(glGetShaderInfoLog(vertex, lenght, &lenght, &error[0]));

				std::cout << "Failed to compile vertex shader!" << std::endl << &error[0] << std::endl;
				GlCall(glDeleteShader(vertex));

				return 0;
			}

			GlCall(glShaderSource(fragment, 1, &fragSrc, nullptr));
			GlCall(glCompileShader(fragment));
			
			GlCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &res));

			if (res == GL_FALSE)
			{
				GLint lenght;

				GlCall(glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &lenght));
				std::vector<char> error(lenght);
				GlCall(glGetShaderInfoLog(fragment, lenght, &lenght, &error[0]));

				std::cout << "Failed to compile fragment shader!" << std::endl << &error[0] << std::endl;
				GlCall(glDeleteShader(fragment));

				return 0;
			}

			GlCall(glAttachShader(program, vertex));
			GlCall(glAttachShader(program, fragment));
			
			GlCall(glLinkProgram(program));

			//GLint isLinked = GL_TRUE;
			GlCall(glGetProgramiv(program, GL_LINK_STATUS, &res));

			if (res == GL_FALSE) {
				GLint lenght;

				GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght));
				std::vector<char> error(lenght);
				GlCall(glGetProgramInfoLog(program, lenght, &lenght, &error[0]));

				std::cout << "Failed to Link shaders!" << std::endl << &error[0] << std::endl;

				GlCall(glDetachShader(program, vertex));
				GlCall(glDeleteShader(vertex));
				
				GlCall(glDetachShader(program, fragment));
				GlCall(glDeleteShader(fragment));
				
				GlCall(glDeleteProgram(program));

				return 0;
			}

			GlCall(glValidateProgram(program));
			
			GlCall(glDetachShader(program, vertex));
			GlCall(glDeleteShader(vertex));
			
			GlCall(glDetachShader(program, fragment));
			GlCall(glDeleteShader(fragment));

			return program;
		}

		GLint Shader::getUniformLocation(const GLchar * name,bool isUniformBlock)
		{
			std::string key(name);

			auto elem = m_uniformMap.find(key);

			if (elem == m_uniformMap.end()) {

				GLint result = 0;
				
				if (isUniformBlock)
					GlCall(result = glGetUniformBlockIndex(m_shaderId, name));

				else 
					GlCall(result = glGetUniformLocation(m_shaderId, name));

				m_uniformMap.insert({ key, result});
				return result;
			}
			else
				return elem->second;
		}

	}
}
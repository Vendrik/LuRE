#include "Shader.h"

#include "..\..\utils\file_utils.h"
#include "..\..\utils\Log.h"
#include "..\..\utils\Debug.h"

#include <iostream>
#include <string>
#include <vector>

namespace lumi {
	namespace graphics {

		Shader::Shader(const std::string& VertexPath, 
					   const std::string& FragmentPath,
					   const std::string& GeometryPath,
					   const std::string& TessControlPath, 
					   const std::string& TessEvaluationPath) :
			m_programId(0), m_separable(false)
		{
			GlCall(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_maxUniformBlockBindings));

			unsigned int tessControlId = 0;
			unsigned int tessEvalId = 0;
			unsigned int geometryId = 0;

			std::string vertexSource = read_file(VertexPath.c_str());
			std::string fragmentSource = read_file(FragmentPath.c_str());

			unsigned int vertexId = compileShader(vertexSource, ShaderType::VertexShader);
			unsigned int fragmentId = compileShader(fragmentSource, ShaderType::FragmentShader);


			if (!TessControlPath.empty())
			{
				std::string tessControlSource = read_file(TessControlPath.c_str());
				tessControlId = compileShader(tessControlSource, ShaderType::TesselationControlShader);
			}

			if (!TessEvaluationPath.empty())
			{
				std::string tessEvalSource = read_file(TessEvaluationPath.c_str());
				tessEvalId = compileShader(tessEvalSource, ShaderType::TesselationEvaluationShader);
			}

			if (!GeometryPath.empty())
			{
				std::string geometrySource = read_file(GeometryPath.c_str());
				geometryId = compileShader(geometrySource, ShaderType::GeometryShader);
			}

			m_programId = linkShader(vertexId, fragmentId, geometryId, tessControlId, tessEvalId);
		}

		Shader::Shader(const std::string & ShaderPath, ShaderType Type) :
			m_programId(0), m_separable(Type == ShaderType::ComputeShader ? false : true)
		{
			GlCall(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_maxUniformBlockBindings));

			std::string shaderSouce = read_file(ShaderPath.c_str());
			unsigned int shaderId = compileShader(shaderSouce, Type);

			if (shaderId != 0)
			{
				m_programId = linkShader(shaderId, Type == ShaderType::ComputeShader ? false : true);
			}

		}

		Shader::~Shader()
		{
			GlCall(glDeleteProgram(m_programId));
		}

		void Shader::setUniform1f(const std::string& Name, float Value)
		{
			GlCall(glUniform1f(getUniformLocation(Name), Value));
		}

		void Shader::setUniform1fv(const std::string& Name, float * Value, int Count)
		{
			GlCall(glUniform1fv(getUniformLocation(Name), Count, Value));
		}

		void Shader::setUniform1i(const std::string& Name, int Value)
		{
			GlCall(glUniform1i(getUniformLocation(Name), Value));
		}

		void Shader::setUniform2i(const std::string& Name, int* Value)
		{
			GlCall(glUniform2i(getUniformLocation(Name), Value[0], Value[1]));
		}

		void Shader::setUniform3i(const std::string& Name, int* Value)
		{
			GlCall(glUniform3i(getUniformLocation(Name), Value[0], Value[1], Value[2]));
		}

		void Shader::setUniform4i(const std::string& Name, int* Value)
		{
			GlCall(glUniform4i(getUniformLocation(Name), Value[0], Value[1], Value[2], Value[3]));
		}

		void Shader::setUniform1iv(const std::string& Name, int * Value, int Count)
		{
			GlCall(glUniform1iv(getUniformLocation(Name), Count, Value));
		}

		void Shader::setUniform2f(const std::string& Name, float* Vector)
		{
			GlCall(glUniform2f(getUniformLocation(Name), Vector[0], Vector[1]));
		}

		void Shader::setUniform2f(const std::string& Name, const maths::vec2 & Vector)
		{
			GlCall(glUniform2f(getUniformLocation(Name), Vector.x, Vector.y));
		}

		void Shader::setUniform3f(const std::string& Name, const maths::vec3 & Vector)
		{
			GlCall(glUniform3f(getUniformLocation(Name), Vector.x, Vector.y, Vector.z));
		}

		void Shader::setUniform4f(const std::string& Name, const maths::vec4 & Vector)
		{
			GlCall(glUniform4f(getUniformLocation(Name), Vector.x, Vector.y, Vector.z, Vector.w));
		}

		void Shader::setUniformMat4(const std::string& Name, const maths::mat4 & matrix)
		{
			GlCall(glUniformMatrix4fv(getUniformLocation(Name), 1, GL_FALSE, matrix.elements));
		}

		void Shader::setUniformBlockBinding(const std::string& Name, unsigned int BindingPoint)
		{
			if (BindingPoint >= m_maxUniformBlockBindings)
				LUMI_FATAL("Bind overflow, maximum allowed: %d, binding: %d", m_maxUniformBlockBindings, BindingPoint);

			GlCall(glUniformBlockBinding(m_programId, getUniformLocation(Name, true), BindingPoint));
		}

		void Shader::enable() const
		{
			GlCall(glUseProgram(m_programId));
		}

		void Shader::disable() const
		{
			GlCall(glUseProgram(0));
		}

		Shader * Shader::FromFile(const std::string& VertexPath, const std::string& FragmentPath)
		{
			return new Shader(VertexPath, FragmentPath);
		}

		unsigned int Shader::compileShader(const std::string& ShaderSoruce, ShaderType Type)
		{
			unsigned int shaderId = 0;
			const char * shaderSourcePtr = ShaderSoruce.c_str();

			GlCall(shaderId = glCreateShader(Type));
			GlCall(glShaderSource(shaderId, 1, &shaderSourcePtr, nullptr));
			GlCall(glCompileShader(shaderId));

			int res;
			GlCall(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res));

			if (res == GL_FALSE)
			{
				int lenght;

				GlCall(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &lenght));
				std::vector<char> error(lenght);
				GlCall(glGetShaderInfoLog(shaderId, lenght, &lenght, &error[0]));

				std::cout << "Failed to compile shader!" << std::endl << &error[0] << std::endl;
				GlCall(glDeleteShader(shaderId));

				return 0;
			}

			return shaderId;
		}

		unsigned int Shader::linkShader(unsigned int ShaderId, bool Separable)
		{
			unsigned int program = 0;

			GlCall(program = glCreateProgram());

			if (Separable)
				GlCall(glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE));

			GlCall(glAttachShader(program, ShaderId));
			GlCall(glLinkProgram(program));

			int res;
			GlCall(glGetProgramiv(program, GL_LINK_STATUS, &res));

			if (res == GL_TRUE)
			{
				GlCall(glValidateProgram(program));
			}
			else
			{
				int lenght;

				GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght));
				std::vector<char> error(lenght);
				GlCall(glGetProgramInfoLog(program, lenght, &lenght, &error[0]));

				std::cout << "Failed to Link shaders!" << std::endl << &error[0] << std::endl;

			}

			GlCall(glDetachShader(program, ShaderId));
			GlCall(glDeleteShader(ShaderId));

			if (res == GL_FALSE)
			{
				GlCall(glDeleteProgram(program));
				program = 0;
			}

			return program;
		}

		unsigned int Shader::linkShader(unsigned int VertexId, unsigned int FragmentId, unsigned int GeometryId, unsigned int TessControlId, unsigned int TessEvaluationId)
		{
			unsigned int program = 0;

			GlCall(program = glCreateProgram());
			GlCall(glAttachShader(program, VertexId));
			GlCall(glAttachShader(program, FragmentId));

			if (GeometryId != 0)
				GlCall(glAttachShader(program, GeometryId));

			if (TessControlId != 0)
				GlCall(glAttachShader(program, TessControlId));

			if (TessEvaluationId != 0)
				GlCall(glAttachShader(program, TessEvaluationId));

			GlCall(glLinkProgram(program));

			int res;
			GlCall(glGetProgramiv(program, GL_LINK_STATUS, &res));

			if (res == GL_TRUE) 
			{
				GlCall(glValidateProgram(program));
			}
			else
			{
				int lenght;

				GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &lenght));
				std::vector<char> error(lenght);
				GlCall(glGetProgramInfoLog(program, lenght, &lenght, &error[0]));

				std::cout << "Failed to Link shaders!" << std::endl << &error[0] << std::endl;

			}

			GlCall(glDetachShader(program, VertexId));
			GlCall(glDeleteShader(VertexId));

			GlCall(glDetachShader(program, FragmentId));
			GlCall(glDeleteShader(FragmentId));

			if (GeometryId != 0)
			{
				GlCall(glDetachShader(program, GeometryId));
				GlCall(glDeleteShader(GeometryId));
			}

			if (TessControlId != 0)
			{
				GlCall(glDetachShader(program, TessControlId));
				GlCall(glDeleteShader(TessControlId));
			}

			if (TessEvaluationId != 0)
			{
				GlCall(glDetachShader(program, TessEvaluationId));
				GlCall(glDeleteShader(TessEvaluationId));
			}

			if (res == GL_FALSE) 
			{
				GlCall(glDeleteProgram(program));
				program = 0;
			}

			return program;
		}

		int Shader::getUniformLocation(const std::string& Name, bool IsUniformBlock)
		{
			auto elem = m_uniformMap.find(Name);

			if (elem == m_uniformMap.end()) {

				GLint result = 0;

				if (IsUniformBlock)
					GlCall(result = glGetUniformBlockIndex(m_programId, Name.c_str()));

				else
					GlCall(result = glGetUniformLocation(m_programId, Name.c_str()));

				m_uniformMap.insert({ Name, result });
				return result;
			}
			else
				return elem->second;
		}
		
	}
}
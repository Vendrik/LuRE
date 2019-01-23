#pragma once

#include<GL\glew.h>
#include "..\..\maths\maths.h"

#include <string>
#include <unordered_map>

namespace lumi {
	namespace graphics {

		enum ShaderType
		{
			VertexShader = GL_VERTEX_SHADER,
			TesselationControlShader = GL_TESS_CONTROL_SHADER,
			TesselationEvaluationShader = GL_TESS_EVALUATION_SHADER,
			GeometryShader = GL_GEOMETRY_SHADER,
			FragmentShader = GL_FRAGMENT_SHADER,
			ComputeShader = GL_COMPUTE_SHADER
		};

		class Shader
		{
		private:
			unsigned int m_programId;
			int m_maxUniformBlockBindings;
			bool m_separable;

			std::unordered_map<std::string, int> m_uniformMap;

		public:

			Shader(const std::string& ShaderPath, ShaderType Type);
			Shader(const std::string& VertexPath, const std::string& FragmentPath, const std::string& GeometryPath = "", const std::string& TessControlPath = "", const std::string& TessEvaluationPath = "");

			~Shader();

			void setUniform1f(const std::string& Name, float Value);
			void setUniform1fv(const std::string& Name, float* Value, int Count);
			void setUniform1i(const std::string& Name, int Value);
			void setUniform2i(const std::string& Name, int* Value);
			void setUniform3i(const std::string& Name, int* Value);
			void setUniform4i(const std::string& Name, int* Value);
			void setUniform1iv(const std::string& Name, int* Value, int Count);
			void setUniform2f(const std::string& Name, const maths::vec2& Vector);
			void setUniform2f(const std::string& Name, float* Vector);
			void setUniform3f(const std::string& Name, const maths::vec3& Vector);
			void setUniform4f(const std::string& Name, const maths::vec4& Vector);
			void setUniformMat4(const std::string& Name, const maths::mat4& Matrix);

			void setUniformBlockBinding(const std::string& Name, unsigned int BindingPoint);

			void enable() const;
			void disable() const;

			static Shader* FromFile(const std::string& VertexPath, const std::string& FragmentPath);

		private:

			static unsigned int compileShader(const std::string& ShaderSource, ShaderType Type);
			static unsigned int linkShader(unsigned int ShaderId, bool Separable);
			static unsigned int linkShader(unsigned int VertexId, unsigned int FragmentId, unsigned int GeometryId = 0, unsigned int TessControlId = 0, unsigned int TessEvaluationId = 0);

			int getUniformLocation(const std::string& Name, bool IsUniformBlock = false);

		};
	}
}
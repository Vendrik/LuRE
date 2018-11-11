#include "ShaderStorageBuffer.h"

#include <GL\glew.h>
#include "..\..\utils\Debug.h"

namespace lumi {
	namespace graphics {


		ShaderStorageBuffer::ShaderStorageBuffer(unsigned int bindingPoint) :
			m_bindingPoint(bindingPoint)
		{
			GlCall(glGenBuffers(1, &m_id));
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
			GlCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingPoint, m_id));
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
		}

		ShaderStorageBuffer::ShaderStorageBuffer(ShaderStorageBuffer && other)
		{
			m_id = other.m_id;
			m_bindingPoint = other.m_bindingPoint;
			m_size = other.m_size;

			other.m_id = 0;

		}

		ShaderStorageBuffer::~ShaderStorageBuffer()
		{
			GlCall(glDeleteBuffers(1, &m_id));
		}

		ShaderStorageBuffer & ShaderStorageBuffer::operator=(ShaderStorageBuffer && other)
		{
			if (this != &other)
			{
				m_id = other.m_id;
				m_bindingPoint = other.m_bindingPoint;
				m_size = other.m_size;

				other.m_id = 0;
			}

			return *this;
		}

		void * ShaderStorageBuffer::mapData(unsigned int count, size_t elem_size)
		{
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));

			if (m_size != count * elem_size)
				GlCall(glBufferData(GL_SHADER_STORAGE_BUFFER, count * elem_size, nullptr, GL_DYNAMIC_DRAW));

			void* ptr = nullptr;

			GlCall(ptr = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY));

			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

			m_size = count * elem_size;

			return ptr;
		}

		void ShaderStorageBuffer::unmapData()
		{
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
			GlCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
		}

		void ShaderStorageBuffer::bind()
		{
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
		}

		void ShaderStorageBuffer::unbind()
		{
			GlCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
		}

	}
}
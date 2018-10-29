#include "UniformBuffer.h"

#include <GL\glew.h>
#include "..\..\utils\Debug.h"


namespace lumi {
	namespace graphics {

		UniformBuffer::UniformBuffer(unsigned int bindingPoint) : m_size(0), m_bindingPoint(bindingPoint)
		{
			GlCall(glGenBuffers(1, &m_id));
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));
			GlCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingPoint, m_id));
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		}

		UniformBuffer::UniformBuffer(UniformBuffer && other)
		{
			this->m_id = other.m_id;
			this->m_size = other.m_size;

			other.m_id = 0;

		}

		UniformBuffer::~UniformBuffer()
		{
			GlCall(glDeleteBuffers(1, &m_id));
		}

		UniformBuffer & UniformBuffer::operator=(UniformBuffer && other)
		{
			if (this != &other)
			{
				this->m_id = other.m_id;
				this->m_size = other.m_size;

				other.m_id = 0;
			}

			return *this;
		}

		void * UniformBuffer::mapData(unsigned int count, size_t elem_size)
		{
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));

			if (m_size != count * elem_size)
				GlCall(glBufferData(GL_UNIFORM_BUFFER, count * elem_size, nullptr, GL_DYNAMIC_DRAW));

			void* ptr = nullptr;

			GlCall(ptr = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));

			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

			m_size = count * elem_size;

			return ptr;
		}

		void UniformBuffer::unmapData()
		{
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));
			GlCall(glUnmapBuffer(GL_UNIFORM_BUFFER));
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		}

		void UniformBuffer::bind()
		{
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));
		}

		void UniformBuffer::unbind()
		{
			GlCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
		}

	}
}
#include "Vertexbuffer.h"

#include <GL\glew.h>

#include "..\..\utils\Debug.h"

namespace lumi {
	namespace graphics {

		VertexBuffer::VertexBuffer()
			: m_size(0), m_componentType(VertexBufferType::Invalid)
		{
			GlCall(glGenBuffers(1, &m_vboID));
		}

		VertexBuffer::VertexBuffer(const VertexBuffer & vbo)
			: m_size(vbo.m_size), m_componentType(vbo.m_componentType)
		{
			GlCall(glGenBuffers(1, &m_vboID));

			if (m_size > 0) {
				GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
				GlCall(glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, vbo.m_vboID));
				GlCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, 0));
				GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}
		}

		VertexBuffer::VertexBuffer(VertexBuffer && vbo)
			: m_vboID(vbo.m_vboID), m_size(vbo.m_size), m_componentType(vbo.m_componentType)
		{
			vbo.m_vboID = 0;
			vbo.m_size = 0;
			vbo.m_componentType = VertexBufferType::Invalid;
		}

		VertexBuffer::VertexBuffer(float * data, unsigned int count)
			: m_size(count * sizeof(float)), m_componentType(VertexBufferType::Scalar)
		{
			GlCall(glGenBuffers(1, &m_vboID));
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
			GlCall(glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW));
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		VertexBuffer::VertexBuffer(float * data, unsigned int count, VertexBufferType componentType)
			: m_size(count * sizeof(float)), m_componentType(componentType)
		{
			GlCall(glGenBuffers(1, &m_vboID));
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
			GlCall(glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW));
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		VertexBuffer::~VertexBuffer()
		{
			GlCall(glDeleteBuffers(1, &m_vboID));
		}

		VertexBuffer & VertexBuffer::operator=(VertexBuffer & other)
		{
			if (this != &other) {
				m_size = other.m_size;
				m_componentType = other.m_componentType;

				GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
				GlCall(glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STATIC_DRAW));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, other.m_vboID));
				GlCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_size));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, 0));
				GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}

			return *this;
		}

		VertexBuffer & VertexBuffer::operator=(VertexBuffer && other)
		{
			if (this != &other) {
				GlCall(glDeleteBuffers(1, &m_vboID));

				this->m_vboID = other.m_vboID;
				other.m_vboID = 0;

			}

			return *this;
		}

		void VertexBuffer::submitData(float * data, unsigned int count, VertexBufferType componentType)
		{
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
			GlCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW));
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

			m_size = count * sizeof(float);
			m_componentType = componentType;
		}

		void * VertexBuffer::mapData(unsigned int count, size_t elem_size)
		{
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
			GlCall(glBufferData(GL_ARRAY_BUFFER, count * elem_size, nullptr, GL_STATIC_DRAW));

			void* ptr = nullptr; 
			
			GlCall(ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

			GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

			m_size = count * elem_size;

			return ptr;
		}

		void VertexBuffer::unmapData()
		{
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
			GlCall(glUnmapBuffer(GL_ARRAY_BUFFER));
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		void VertexBuffer::bind() const
		{
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_vboID));
		}

		void VertexBuffer::unbind() const
		{
			GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

	}
}

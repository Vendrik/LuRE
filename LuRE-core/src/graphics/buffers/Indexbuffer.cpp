#include "Indexbuffer.h"

#include <GL\glew.h>

#include "..\..\utils\Debug.h"

namespace lumi {
	namespace graphics {

		IndexBuffer::IndexBuffer()
			: m_indexCount(0)
		{
			GlCall(glGenBuffers(1, &m_iboID));
		}

		IndexBuffer::IndexBuffer(const IndexBuffer & ibo)
			: m_indexCount(ibo.m_indexCount)
		{
			GlCall(glGenBuffers(1, &m_iboID));

			if (m_indexCount > 0) {
				GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
				GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), nullptr, GL_STATIC_DRAW));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, ibo.m_iboID));
				GlCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_indexCount * sizeof(unsigned int)));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, 0));
				GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			}
		}

		IndexBuffer::IndexBuffer(IndexBuffer && ibo)
			: m_iboID(ibo.m_iboID), m_indexCount(ibo.m_indexCount)
		{
			ibo.m_iboID = 0;
			ibo.m_indexCount = 0;
		}

		IndexBuffer::IndexBuffer(unsigned int * data, unsigned int count)
			: m_indexCount(count)
		{
			GlCall(glGenBuffers(1, &m_iboID));
			GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
			GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
			GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		IndexBuffer::~IndexBuffer()
		{
			GlCall(glDeleteBuffers(1, &m_iboID));
		}

		IndexBuffer & IndexBuffer::operator=(IndexBuffer & other)
		{
			if (this != &other) {
				m_indexCount = other.m_indexCount;

				GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
				GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), nullptr, GL_STATIC_DRAW));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, other.m_iboID));
				GlCall(glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_indexCount * sizeof(unsigned int)));
				
				GlCall(glBindBuffer(GL_COPY_READ_BUFFER, 0));
				GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			}

			return *this;
		}

		IndexBuffer & IndexBuffer::operator=(IndexBuffer && other)
		{
			if (this != &other) {
				GlCall(glDeleteBuffers(1, &m_iboID));

				this->m_iboID = other.m_iboID;
				this->m_indexCount = other.m_indexCount;

				other.m_iboID = 0;
				other.m_indexCount = 0;
			}

			return *this;
		}

		void IndexBuffer::submitData(unsigned int * data, unsigned int count)
		{
			GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
			GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
			GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

			m_indexCount = count;
		}

		void IndexBuffer::bind() const
		{
			GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID));
		}

		void IndexBuffer::unbind() const
		{
			GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

	}
}
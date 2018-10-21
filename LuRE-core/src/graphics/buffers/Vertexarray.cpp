#include "Vertexarray.h"

#include <GL\glew.h>

#include "..\..\utils\Debug.h"

namespace lumi {
	namespace graphics {

		VertexArray::VertexArray()
		{
			GlCall(glGenVertexArrays(1, &m_vaoID));
		}

		VertexArray::VertexArray(VertexArray && vao)
			: m_vaoID(vao.m_vaoID)
		{
			vao.m_vaoID = 0;
		}

		VertexArray::~VertexArray()
		{
			GlCall(glDeleteVertexArrays(1, &m_vaoID));
		}

		VertexArray & VertexArray::operator=(VertexArray && other)
		{
			if (this != &other) {
				GlCall(glDeleteVertexArrays(1, &m_vaoID));

				this->m_vaoID = other.m_vaoID;
				other.m_vaoID = 0;
			}

			return *this;
		}

		void VertexArray::addBuffer(VertexBuffer* buffer, BufferType index, size_t stride, void* offset, unsigned int instanceDivisor)
		{
			GlCall(glBindVertexArray(m_vaoID));
			buffer->bind();

			unsigned int components = buffer->getComponentCount();

			if (components <= 4)
			{
				GlCall(glEnableVertexAttribArray(index));
				GlCall(glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE, stride, offset));

				if (instanceDivisor > 0)
					GlCall(glVertexAttribDivisor(index, instanceDivisor));
			}
			else if (buffer->getComponentType() == VertexBufferType::Mat4)
			{
				size_t off = sizeof(float) * 4;

				GlCall(glEnableVertexAttribArray(index));
				GlCall(glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, offset));
				GlCall(glEnableVertexAttribArray(index + 1));
				GlCall(glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 1)));
				GlCall(glEnableVertexAttribArray(index + 2));
				GlCall(glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 2)));
				GlCall(glEnableVertexAttribArray(index + 3));
				GlCall(glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 3)));

				if (instanceDivisor > 0)
				{
					GlCall(glVertexAttribDivisor(index + 0, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 1, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 2, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 3, instanceDivisor));
				}
		

			}

			buffer->unbind();
			GlCall(glBindVertexArray(0));
		}

		void VertexArray::addBuffer(const VertexBuffer & buffer, BufferType index, size_t stride, void* offset, unsigned int instanceDivisor)
		{
			GlCall(glBindVertexArray(m_vaoID));
			buffer.bind();

			unsigned int components = buffer.getComponentCount();

			if (components <= 4)
			{
				GlCall(glEnableVertexAttribArray(index));
				GlCall(glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE, stride, offset));
				
				if (instanceDivisor > 0)
					GlCall(glVertexAttribDivisor(index, instanceDivisor));
			}
			else if (buffer.getComponentType() == VertexBufferType::Mat4)
			{
				size_t off = sizeof(float) * 4;

				GlCall(glEnableVertexAttribArray(index));
				GlCall(glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, offset));
				GlCall(glEnableVertexAttribArray(index + 1));
				GlCall(glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 1)));
				GlCall(glEnableVertexAttribArray(index + 2));
				GlCall(glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 2)));
				GlCall(glEnableVertexAttribArray(index + 3));
				GlCall(glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 3)));

				if (instanceDivisor > 0)
				{
					GlCall(glVertexAttribDivisor(index + 0, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 1, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 2, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 3, instanceDivisor));
				}
			}

			buffer.unbind();
			GlCall(glBindVertexArray(0));
		}

		void VertexArray::addBuffer(const VertexBuffer & buffer, BufferType index, unsigned int components, size_t stride, void * offset, unsigned int instanceDivisor)
		{
			GlCall(glBindVertexArray(m_vaoID));
			buffer.bind();

			if (components <= 4)
			{
				GlCall(glEnableVertexAttribArray(index));
				GlCall(glVertexAttribPointer(index, components, GL_FLOAT, GL_FALSE, stride, offset));

				if (instanceDivisor > 0)
					GlCall(glVertexAttribDivisor(index, instanceDivisor));
			}
			else if (components == VertexBufferType::Mat4)
			{
				size_t off = sizeof(float) * 4;

				GlCall(glEnableVertexAttribArray(index));
				GlCall(glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, offset));
				GlCall(glEnableVertexAttribArray(index + 1));
				GlCall(glVertexAttribPointer(index + 1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 1)));
				GlCall(glEnableVertexAttribArray(index + 2));
				GlCall(glVertexAttribPointer(index + 2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 2)));
				GlCall(glEnableVertexAttribArray(index + 3));
				GlCall(glVertexAttribPointer(index + 3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(off * 3)));

				if (instanceDivisor > 0)
				{
					GlCall(glVertexAttribDivisor(index + 0, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 1, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 2, instanceDivisor));
					GlCall(glVertexAttribDivisor(index + 3, instanceDivisor));
				}
			}

			buffer.unbind();
			GlCall(glBindVertexArray(0));
		}

		void VertexArray::bind() const
		{
			GlCall(glBindVertexArray(m_vaoID));
		}

		void VertexArray::unbind() const
		{
			GlCall(glBindVertexArray(0));
		}

	}
}


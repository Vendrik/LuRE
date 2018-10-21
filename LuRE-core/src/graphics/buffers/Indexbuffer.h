#pragma once

namespace lumi {
	namespace graphics {

		class IndexBuffer
		{
		private:

			unsigned int m_iboID;
			unsigned int m_indexCount;

		public:
			IndexBuffer();
			IndexBuffer(const IndexBuffer& ibo);
			IndexBuffer(IndexBuffer&& ibo);
			IndexBuffer(unsigned int* data, unsigned int count);
			~IndexBuffer();

			IndexBuffer& operator=(IndexBuffer& other);
			IndexBuffer& operator=(IndexBuffer&& other);

			void submitData(unsigned int* data, unsigned int count);

			void bind() const;
			void unbind() const;

			inline unsigned int getIndexCount() const { return m_indexCount; };
		};

	}
}
#pragma once

namespace lumi {
	namespace graphics {

		enum VertexBufferType : unsigned int {
			Invalid = 0,
			Scalar,
			Vec2,
			Vec3,
			Vec4,
			Mat4

		};

		class VertexBuffer
		{
		private:

			unsigned int m_vboID;
			size_t m_size;
			VertexBufferType m_componentType;

		public:
			VertexBuffer();
			VertexBuffer(const VertexBuffer& vbo);
			VertexBuffer(VertexBuffer&& vbo);
			VertexBuffer(float* data, unsigned int count);
			VertexBuffer(float* data, unsigned int count, VertexBufferType componentType);
			~VertexBuffer();

			VertexBuffer& operator=(VertexBuffer& other);
			VertexBuffer& operator=(VertexBuffer&& other);

			void submitData(float* data, unsigned int count, VertexBufferType componentType);
			void* mapData(unsigned int count, size_t elem_size);
			void unmapData();

			void bind() const;
			void unbind() const;

			inline unsigned int getComponentCount() const { return m_componentType <= 4 ? m_componentType : 16; };
			inline VertexBufferType getComponentType() const { return m_componentType; }

		};

	}
}

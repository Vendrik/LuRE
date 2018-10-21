#pragma once

#include "Vertexbuffer.h"

namespace lumi {
	namespace graphics {

		enum BufferType : unsigned int {
			Positions = 0,
			Uvs,
			Normals,
			InstanceOffset
		};

		class VertexArray
		{
		private:
			unsigned int m_vaoID;

		public:

			VertexArray();
			VertexArray(const VertexArray& vao) = delete;
			VertexArray(VertexArray&& vao);
			~VertexArray();

			VertexArray& operator=(VertexArray& other) = delete;
			VertexArray& operator=(VertexArray&& other);

			void addBuffer(VertexBuffer* buffer, BufferType index, size_t stride = 0, void* offset = nullptr, unsigned int instanceDivisor = 0);
			void addBuffer(const VertexBuffer& buffer, BufferType index, size_t stride = 0, void* offset = nullptr, unsigned int instanceDivisor = 0);
			void addBuffer(const VertexBuffer& buffer, BufferType index, unsigned int components, size_t stride = 0, void* offset = nullptr, unsigned int instanceDivisor = 0);

			void bind() const;
			void unbind() const;

		};
	}
}
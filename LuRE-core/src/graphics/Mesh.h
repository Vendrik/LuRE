#pragma once

#include <vector>

#include "buffers\Vertexarray.h"
#include "buffers\Vertexbuffer.h"
#include "buffers\Indexbuffer.h"

namespace lumi {
	namespace graphics {

		/*struct MeshModel {
			float* positions;
			float* normals;
			float* uvs;

			unsigned int* indices;

			size_t positionsCount;
			size_t normalsCount;
			size_t uvsCount;

			size_t indicesCount;
		};

		struct VertexData {
			std::vector<float> positions;
			std::vector<float> normals;
			std::vector<float> uvs;

			std::vector<unsigned int> indices;
		};*/

		class Mesh
		{
		private:

			struct VertexData {
				float x, y, z;
				float u, v;
				float nx, ny, nz;
			};
	
			std::vector<float> m_positions;
			std::vector<float> m_normals;
			std::vector<float> m_uvs;
			std::vector<unsigned int> m_indices;
			
			VertexBuffer m_vboPos;
			VertexBuffer m_vboNorms;
			VertexBuffer m_vboUvs;

			VertexBuffer m_interleaved;

			IndexBuffer m_ibo;

			bool m_initializedFlag;

		public:
			Mesh();
			Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, bool initialize = false);
			Mesh(const Mesh& mesh);
			Mesh(Mesh&& mesh);
			~Mesh();

			Mesh& operator=(Mesh& other);
			Mesh& operator=(Mesh&& other);

			inline const VertexBuffer& getPositionBuffer() const { return m_vboPos; };
			inline const VertexBuffer& getNormalBuffer() const { return m_vboNorms; };
			inline const VertexBuffer& getUvBuffer() const { return m_vboUvs; };

			inline const IndexBuffer& getIndexBuffer() const { return m_ibo; };

			inline const unsigned int getIndexCount() const { return m_ibo.getIndexCount(); };

			void initializeInterleavedVAO(VertexArray& vao);

		private:
			void setupMesh();
		};
	}
}




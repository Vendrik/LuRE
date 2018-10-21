#include "Mesh.h"

#include "buffers\Vertexbuffer.h"
#include "..\utils\Log.h"


namespace lumi {
	namespace graphics {
		Mesh::Mesh()
			: m_initializedFlag(false)
		{
		}

		Mesh::Mesh(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& uvs, const std::vector<unsigned int>& indices, bool initialize)
			: m_positions(positions), m_normals(normals), m_uvs(uvs), m_indices(indices), m_initializedFlag(initialize)
		{
			if (m_initializedFlag)
				setupMesh();
		}

		Mesh::Mesh(const Mesh & mesh)
			: m_positions(mesh.m_positions), m_normals(mesh.m_normals), m_uvs(mesh.m_uvs), m_indices(mesh.m_indices), m_initializedFlag(mesh.m_initializedFlag),
			m_vboPos(mesh.m_vboPos), m_vboNorms(mesh.m_vboNorms), m_vboUvs(mesh.m_vboUvs), m_interleaved(mesh.m_interleaved), m_ibo(mesh.m_ibo)
		{
			LUMI_INFO("Mesh copy constructor");
			setupMesh();
		}

		Mesh::Mesh(Mesh && mesh)
			: m_positions(std::move(mesh.m_positions)), m_normals(std::move(mesh.m_normals)), m_uvs(std::move(mesh.m_uvs)), m_indices(std::move(mesh.m_indices)), m_initializedFlag(mesh.m_initializedFlag),
			m_vboPos(std::move(mesh.m_vboPos)), m_vboNorms(std::move(mesh.m_vboNorms)), m_vboUvs(std::move(mesh.m_vboUvs)), m_ibo(std::move(mesh.m_ibo))
		{
			LUMI_INFO("Mesh move constructor");
		}

		Mesh::~Mesh()
		{
			LUMI_INFO("Mesh is dying");
		}

		Mesh & Mesh::operator=(Mesh & other)
		{
			if (this != &other) {
				m_positions = other.m_positions;
				m_normals = other.m_normals;
				m_indices = other.m_indices;
				m_uvs = other.m_uvs;

				m_vboNorms = other.m_vboNorms;
				m_vboPos = other.m_vboPos;
				m_vboUvs = other.m_vboUvs;
				m_ibo = other.m_ibo;
				m_interleaved = other.m_interleaved;
				LUMI_INFO("Mesh copy operator");

			}

			return *this;
		}

		Mesh & Mesh::operator=(Mesh && other)
		{
			if (this != &other) {
				m_positions = std::move(other.m_positions);
				m_normals = std::move(other.m_normals);
				m_indices = std::move(other.m_indices);
				m_uvs = std::move(other.m_uvs);

				m_vboNorms = std::move(other.m_vboNorms);
				m_vboPos = std::move(other.m_vboPos);
				m_vboUvs = std::move(other.m_vboUvs);
				m_ibo = std::move(other.m_ibo);
				m_interleaved = std::move(other.m_interleaved);
				LUMI_INFO("Mesh move operator");

			}

			return *this;
		}

		void Mesh::initializeInterleavedVAO(VertexArray & vao)
		{

			vao.addBuffer(m_interleaved, BufferType::Positions, 3, sizeof(VertexData));
			vao.addBuffer(m_interleaved, BufferType::Uvs, 2, sizeof(VertexData), (void*)offsetof(VertexData, VertexData::u));
			vao.addBuffer(m_interleaved, BufferType::Normals, 3, sizeof(VertexData), (void*)offsetof(VertexData, VertexData::nx));
		}

		void Mesh::setupMesh()
		{
			m_vboPos.submitData(m_positions.data(), m_positions.size(), VertexBufferType::Vec3);
			m_vboNorms.submitData(m_normals.data(), m_normals.size(), VertexBufferType::Vec3);
			m_vboUvs.submitData(m_uvs.data(), m_uvs.size(), VertexBufferType::Vec2);

			m_ibo.submitData(m_indices.data(), m_indices.size());

			VertexData* buffer = (VertexData*)m_interleaved.mapData(m_positions.size() + m_normals.size() + m_uvs.size(), sizeof(float));

			for (unsigned int i = 0, j = 0; i < m_positions.size() && j < m_uvs.size(); i += 3, j += 2)
			{
				
				buffer->x = m_positions[i];
				buffer->y = m_positions[i + 1];
				buffer->z = m_positions[i + 2];

				buffer->u = m_uvs[j];
				buffer->v = m_uvs[j + 1];

				buffer->nx = m_normals[i];
				buffer->ny = m_normals[i + 1];
				buffer->nz = m_normals[i + 2];

				buffer++;
			}

			m_interleaved.unmapData();

		}
	}
}


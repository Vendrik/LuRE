#include "Model.h"

#include <fstream>
#include <sstream>

#include "..\utils\Log.h"
#include "..\utils\file_utils.h"

#include "MeshManager.h"
#include "buffers\Vertexarray.h"

namespace lumi {
	namespace graphics {

		Model::Model(MeshManager* meshManager, const std::string & resourceName, const std::string & resourcePath)
			: m_meshManager(meshManager), m_resourceName(resourceName), m_resourcePath(resourcePath), 
			m_position(maths::vec3(0.0f, 0.0f ,0.0f)), m_rotation(maths::vec3(0.0f, 0.0f, 0.0f)), m_scale(1.0f), 
			m_color(0.4f, 0.5f, 1.9f), m_instanced_elements(0)
		{

			m_tranformationMatrix = maths::mat4::transformation(m_position, m_rotation.x, m_rotation.y, m_rotation.z, m_scale);
			m_toUpdate = false;
			m_modelMatrix = std::make_shared<VertexBuffer>((float*)(&m_tranformationMatrix.elements[0]), 16, VertexBufferType::Mat4);
			m_vertexArray = std::make_shared<VertexArray>();

			m_mesh = this->getMesh();
		}

		Model::Model(MeshManager* meshManager, const std::string & resourceName, const std::string & resourcePath, const maths::vec3 & position, const maths::vec3 & rotation, float scale)
			: m_meshManager(meshManager), m_resourceName(resourceName), m_resourcePath(resourcePath), 
			m_position(position), m_rotation(rotation), m_scale(scale), 
			m_color(0.4f, 0.5f, 1.9f), m_instanced_elements(0)
		{

				m_tranformationMatrix = maths::mat4::transformation(m_position, m_rotation, m_scale);
				m_toUpdate = false;
				m_modelMatrix = std::make_shared<VertexBuffer>((float*)(&m_tranformationMatrix.elements[0]), 16, VertexBufferType::Mat4);
				m_vertexArray = std::make_shared<VertexArray>();

				m_mesh = this->getMesh();
		}

		Model::Model(MeshManager* meshManager, const std::string & resourceName, const std::string & resourcePath, std::vector<maths::mat4>& instancedInfo)
			: m_meshManager(meshManager), m_resourceName(resourceName), m_resourcePath(resourcePath), 
			m_position(maths::vec3(0.0f, 0.0f, 0.0f)), m_rotation(maths::vec3(0.0f, 0.0f, 0.0f)), m_scale(1.0f), 
			m_color(0.4f, 0.5f, 1.9f)
		{

			m_modelMatrix = std::make_shared<VertexBuffer>((float*)(&instancedInfo[0]), instancedInfo.size() * 16, VertexBufferType::Mat4);
			m_isInstanced = true;
			m_instanced_elements = instancedInfo.size();
			m_toUpdate = false;
			m_vertexArray = std::make_shared<VertexArray>();

			m_mesh = this->getMesh();
		}

		Model::~Model()
		{

		}

		maths::mat4 Model::getTransformationMatrix()
		{
			if (m_toUpdate) {
				m_tranformationMatrix = maths::mat4::transformation(m_position, m_rotation, m_scale);
				m_toUpdate = false;

			}

			return m_tranformationMatrix;
		}


		std::shared_ptr<Mesh> Model::getMesh()
		{
			if (m_mesh.get() == nullptr)
			{
				m_meshManager->getMesh(m_resourcePath, m_mesh);

				if (m_mesh.get() != nullptr)
				{
					//unsigned int div = m_isInstanced ? 1 : 0;
					m_mesh->initializeInterleavedVAO(*m_vertexArray);
					m_vertexArray->addBuffer(*m_modelMatrix, BufferType::InstanceOffset, sizeof(maths::mat4), nullptr, 1);

					/*
					if (m_isInstanced) 
					{
						m_vertexArray->addBuffer(*m_modelMatrix, BufferType::InstanceOffset, sizeof(maths::mat4), nullptr, 1);
					}
					else 
					{
						m_vertexArray->addBuffer(*m_modelMatrix, BufferType::InstanceOffset, sizeof(maths::mat4), nullptr, 1);
					}
					*/
				}
				// TODO inserire qui il set up del vertex buffer per l'invio dei dati d'istanza (mat4 transform matrix)
			}
				

			return m_mesh;
		}

		void Model::bind()
		{
			m_vertexArray->bind();
			m_mesh->getIndexBuffer().bind();
		}

		void Model::unbind()
		{
			m_mesh->getIndexBuffer().unbind();
			m_vertexArray->unbind();
		}

	}
}


#include "MeshManager.h"

#include <vector>
#include <fstream>
#include <sstream>

#include "..\utils\Log.h"
#include "..\utils\file_utils.h"

namespace lumi {
	namespace graphics {

		MeshManager::MeshManager()
		{
			m_stopFlag = false;

			m_consumerThread = std::thread(&MeshManager::consumeRequests, this);

		}

		MeshManager::~MeshManager()
		{
			m_stopFlag.store(true, std::memory_order_release);
			m_queueCv.notify_all();

			m_consumerThread.join();
		
		}

		bool MeshManager::getMesh(std::string & meshPath, std::shared_ptr<Mesh>& mesh)
		{

			auto stored_mesh = m_storedMeshes.find(meshPath);

			if (stored_mesh != m_storedMeshes.end())
			{
				if (m_storedMeshes[meshPath].ready)
				{
					mesh = m_storedMeshes[meshPath].mesh;
					return true;
				}
				else if (m_storedMeshes[meshPath].loaded.load(std::memory_order_acquire))
				{
					std::shared_ptr<Mesh> new_mesh = std::make_shared<Mesh>(m_storedMeshes[meshPath].data->positions, m_storedMeshes[meshPath].data->normals, m_storedMeshes[meshPath].data->uvs, m_storedMeshes[meshPath].data->indices, true);

					m_storedMeshes[meshPath].mesh = new_mesh;
					m_storedMeshes[meshPath].ready = true;
					
					mesh = new_mesh;
					return true;
				}
				else
					return false;
			}
			
			m_storedMeshes[meshPath];

			{
				std::lock_guard<std::mutex> lock(m_queueMutex);
				m_loadQueue.push(meshPath);
			}
			m_queueCv.notify_one();

			return false;
		}

		void MeshManager::consumeRequests()
		{
			while (!m_stopFlag.load(std::memory_order_acquire)) {
				
				std::string pack;
				{
					std::unique_lock<std::mutex> lock(m_queueMutex);

					while (m_loadQueue.size() == 0 && !m_stopFlag.load(std::memory_order_acquire)) {
						m_queueCv.wait(lock);
					}
					
					if (m_stopFlag.load(std::memory_order_acquire))
						return;

					pack = std::move(m_loadQueue.front());
					m_loadQueue.pop();
				}

				loadMesh(pack);
		
			}
		}

		void MeshManager::loadMesh(std::string& meshPath)
		{
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<float> uvs;
			std::vector<unsigned int> indices;

			float* tmp_normals = nullptr;
			float* tmp_uvs = nullptr;

			std::ifstream inputFile;


			inputFile.open(meshPath.c_str());
			if (!inputFile.good()) {

				LUMI_FATAL("Could not open the current file: %s", meshPath.c_str());
				inputFile.close();
			}

			std::string line;
			bool toAlloc = true;
			while (std::getline(inputFile, line) && !m_stopFlag.load(std::memory_order_acquire))
			{

				std::vector<std::string> tokens = split(line, ' ');

				if (tokens[0].compare("v") == 0) {
					vertices.emplace_back(std::stof(tokens[1]));
					vertices.emplace_back(std::stof(tokens[2]));
					vertices.emplace_back(std::stof(tokens[3]));

				}
				else if (tokens[0].compare("vt") == 0) {
					uvs.emplace_back(std::stof(tokens[1]));
					uvs.emplace_back(1.0f - std::stof(tokens[2]));

				}
				else if (tokens[0].compare("vn") == 0) {
					normals.emplace_back(std::stof(tokens[1]));
					normals.emplace_back(std::stof(tokens[2]));
					normals.emplace_back(std::stof(tokens[3]));

				}
				else if (tokens[0].compare("f") == 0) {

					if (toAlloc) {
						tmp_uvs = (float*)malloc(2 * sizeof(float) * (vertices.size() / 3));
						tmp_normals = (float*)malloc(sizeof(float) * vertices.size());
						toAlloc = false;
					}

					for (int i = 1; i < 4; i++) {
						std::vector<std::string> vertex = split(tokens[i], '/');

						int currentVertexPointer = std::stoi(vertex[0]) - 1;
						indices.emplace_back(currentVertexPointer);

						int currentUvs = std::stoi(vertex[1]) - 1;
						memcpy_s(tmp_uvs + (currentVertexPointer * 2), sizeof(float) * 2, &uvs[currentUvs * 2], sizeof(float) * 2);

						int currentNormal = std::stoi(vertex[2]) - 1;
						memcpy_s(tmp_normals + (currentVertexPointer * 3), sizeof(float) * 3, &normals[currentNormal * 3], sizeof(float) * 3);

					}
				}
			}

			if (m_stopFlag.load(std::memory_order_acquire)) {

				free(tmp_uvs);
				free(tmp_normals);

				return;
			}

			inputFile.close();

			normals.clear();
			uvs.clear();

			normals.assign(tmp_normals, tmp_normals + vertices.size());
			uvs.assign(tmp_uvs, tmp_uvs + (2 * (vertices.size() / 3)));

			free(tmp_uvs);
			free(tmp_normals);

			MeshInfo *mesh = new MeshInfo();
			mesh->positions = std::move(vertices);
			mesh->normals = std::move(normals);
			mesh->uvs = std::move(uvs);
			mesh->indices = std::move(indices);

			m_storedMeshes[meshPath].data = mesh;
			m_storedMeshes[meshPath].loaded.store(true, std::memory_order_release);

		}

	}
}
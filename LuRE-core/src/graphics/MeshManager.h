#pragma once

#include <queue>
#include <unordered_map>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <memory>
#include <vector>

#include "Mesh.h"


namespace lumi {
	namespace graphics {

		class MeshManager
		{

		private:

			struct MeshInfo {
				std::vector<float> positions;
				std::vector<float> normals;
				std::vector<float> uvs;
				std::vector<unsigned int> indices;

			};

			struct MeshData {
				bool ready;
				std::atomic<bool> loaded;
				MeshInfo* data;
				std::shared_ptr<Mesh> mesh;

				MeshData() : ready(false), loaded(false), data(nullptr) {};

			};

			std::queue<std::string> m_loadQueue;
			std::unordered_map<std::string, MeshData> m_storedMeshes;
			std::mutex m_queueMutex;
			std::condition_variable m_queueCv;
			std::thread m_consumerThread;
			std::atomic<bool> m_stopFlag;

		public:
			MeshManager();
			~MeshManager();

			bool getMesh(std::string& meshPath, std::shared_ptr<Mesh>& mesh);

		private:
			void consumeRequests();
			void loadMesh(std::string& meshPath);
		};

	}
}
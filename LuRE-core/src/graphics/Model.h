#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Mesh.h"
#include "..\maths\maths.h"


namespace lumi {
	namespace graphics {

		class MeshManager;

		class Model
		{
		private:
			maths::mat4 m_tranformationMatrix;

			std::shared_ptr<VertexArray> m_vertexArray;
			std::shared_ptr<VertexBuffer> m_modelMatrix;
			unsigned int m_instanced_elements;

			maths::vec3 m_color;
			maths::vec3 m_position;
			maths::vec3 m_rotation;
			float m_scale;

			bool m_toUpdate;
			bool m_isInstanced = false;

			std::string m_resourceName;
			std::string m_resourcePath;

			std::shared_ptr<Mesh> m_mesh;

			MeshManager* m_meshManager;


		public:
			Model(MeshManager* meshManager, const std::string& resourceName, const std::string& resourcePath);
			Model(MeshManager* meshManager, const std::string& resourceName, const std::string& resourcePath, const maths::vec3& position, const maths::vec3& rotation, float scale);
			Model(MeshManager* meshManager, const std::string& resourceName, const std::string& resourcePath, std::vector<maths::mat4>& instancedInfo);
			~Model();

			inline std::shared_ptr<VertexArray> getVertexArray() { return m_vertexArray; }

			maths::mat4 getTransformationMatrix();

			inline const maths::vec3& getColor() const { return m_color; };
			inline const unsigned int getInstancedElementsCount() const { return m_instanced_elements; };
			inline bool isInstanced() const { return m_instanced_elements > 1 ? true : false; };
			inline bool isMeshReady() const { return m_mesh.get() != nullptr; };

			inline void setPosition(const maths::vec3& position) { m_position = position; m_toUpdate = true; };
			inline void setRotation(const maths::vec3& rotation) { m_rotation = rotation; m_toUpdate = true; };
			inline void setScale(const float scale) { m_scale = scale; m_toUpdate = true; };


			std::shared_ptr<Mesh> getMesh();

			void bind();
			void unbind();

		};

	}
}




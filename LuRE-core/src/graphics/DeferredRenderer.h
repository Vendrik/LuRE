#pragma once

#include "Renderer.h"
#include "buffers\Framebuffer.h"
#include "buffers\Vertexarray.h"
#include "buffers\Vertexbuffer.h"
#include "buffers\UniformBuffer.h"
#include "shaders\Shader.h"
#include "..\maths\maths.h"
#include "Light.h"

#include <memory>

namespace lumi {
	namespace graphics {
	
		class DeferredRenderer : public Renderer
		{
		private:
			std::unique_ptr<FrameBuffer> m_GBuffer;
			std::unique_ptr<VertexArray> m_targetVao;
			std::shared_ptr<Shader> m_geometryShader;
			std::shared_ptr<Shader> m_lightingShader;
			std::shared_ptr<Shader> m_shadowShader;
			std::unique_ptr<FrameBuffer> m_shadowMapBuffer;

			VertexBuffer m_targetVbo;
			UniformBuffer m_lightsUbo;

			maths::mat4 m_projectionMatrix;
			maths::mat4 m_lookatMatrix;
			maths::mat4 m_shadowLookAtMatrix;
			maths::vec3 m_cameraPosition;
			maths::vec3 m_cameraTarget;

			unsigned int m_width;
			unsigned int m_height;

		public:
			DeferredRenderer(unsigned int width, unsigned int height, std::shared_ptr<Shader> geometryShader, std::shared_ptr<Shader> lightningShader, std::shared_ptr<Shader> shadowShader);
			
			virtual ~DeferredRenderer();

			void render(std::vector<Model>& renderList) override;
			//void renderShadows(std::vector<Model>& renderList, const maths::vec3& lightPosition);

			void setPorjectionMatrix(const maths::mat4& projectionMatrix);
			void setCameraPosition(const maths::vec3& cameraPosition);
			void setCameraDirection(const maths::vec3& cameraDirection);
			void setViewportSettings(int viewport[4], float depthRange[2]);
			void updateViewportSettings();
			void setCameraLookTarget(const maths::mat4& lookatMatrix);
			void setRenderingTargetSize(unsigned int width, unsigned int height);

			inline void setCameraTarget(const maths::vec3& target) { m_cameraTarget = target; };
			inline void setRenderShader(std::shared_ptr<Shader>& shader) { m_geometryShader.swap(shader); };
			void setLights(std::vector<Light>& lights);

		private:

			void init();
		};
	
	}
}
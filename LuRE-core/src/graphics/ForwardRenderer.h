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

		class ForwardRenderer : public Renderer
		{
		private:

			std::unique_ptr<FrameBuffer> m_targetFbo;
			std::unique_ptr<FrameBuffer> m_multisampledFbo;
			std::unique_ptr<VertexArray> m_targetVao;

			VertexBuffer m_targetVbo;
			UniformBuffer m_lightsUbo;

			maths::mat4 m_projectionMatrix;
			maths::mat4 m_lookatMatrix;
			maths::vec3 m_cameraPosition;
			
			unsigned int m_width;
			unsigned int m_height;
			unsigned char m_antiAliasingLevel;

			std::shared_ptr<Shader> m_postPorcessingShader;
			std::shared_ptr<Shader> m_renderShader;

		public:
			ForwardRenderer(unsigned int width, unsigned int height, std::shared_ptr<Shader> renderShader);
			ForwardRenderer(unsigned int width, unsigned int height, std::shared_ptr<Shader> renderShader, unsigned char antialiasingLevel);
			virtual ~ForwardRenderer();

			void render(std::vector<Model>& renderList) override;

			void setAntiAlisingLevel(unsigned char level);
			inline unsigned char getAntiAliasingLevel() const { return m_antiAliasingLevel; };
			inline bool isAntiAliasingEnable() const { return m_antiAliasingLevel > 0; };

			void setPorjectionMatrix(const maths::mat4& projectionMatrix);
			void setCameraPosition(const maths::vec3& cameraPosition);
			void setCameraLookTarget(const maths::mat4& lookatMatrix);
			void setRenderingTargetSize(unsigned int width, unsigned int height);		

			void setLights(const std::vector<Light>& lights);
			
			inline void setRenderShader(std::shared_ptr<Shader>& shader) { m_renderShader.swap(shader); };
			inline std::shared_ptr<Shader> getShader() { return m_antiAliasingLevel > 0 ? m_postPorcessingShader : m_renderShader; }

		private:

			void init();
		};

	}
}

#include "ForwardRenderer.h"

#include "shaders\ShaderFactory.h"
#include "..\utils\Log.h"
#include "..\utils\Debug.h"


namespace lumi {
	namespace graphics {

		ForwardRenderer::ForwardRenderer(unsigned int width, unsigned int height, std::shared_ptr<Shader> renderShader)
			: m_antiAliasingLevel(0), m_width(width), m_height(height), m_renderShader(renderShader), m_projectionMatrix(1.0f), m_lookatMatrix(1.0f), m_cameraPosition(1.0f, 1.0f, 1.0f)
		{
			float quadVertices[] = 
			{	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
				// positions   // texCoords
			    -1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};
		
			m_targetFbo = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height));
			m_targetFbo->addColorAttachment(TextureFormat::RGB, TextureFormat::RGB, TextureDataType::UNSIGNED_BYTE);
			m_targetFbo->addDepthAttachment(TextureFormat::DEPTH);
			if (!m_targetFbo->isFrameBufferComplete())
			{
				LUMI_WARN("Watch out! Framebuffer target is not complete!");
			}

			m_targetVao = std::unique_ptr<VertexArray>(new VertexArray());

			m_targetVbo.submitData(&quadVertices[0], 24, VertexBufferType::Vec2);

			m_targetVao->addBuffer(m_targetVbo, BufferType::Positions, 4 * sizeof(float), (void*)(nullptr));
			m_targetVao->addBuffer(m_targetVbo, BufferType::Uvs, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			m_postPorcessingShader = std::shared_ptr<Shader>(ShaderFactory::PassthroughShader());

			m_renderShader->enable();
			m_renderShader->setUniform3f("lightPos", maths::vec3(0.0f, 0.0f, 35.0f));
			m_renderShader->setUniform3f("lightColor", maths::vec3(1.0f, 1.0f, 1.0f));
			m_renderShader->setUniform3f("objectColor", maths::vec3(0.4f, 0.5f, 1.9f));
			m_renderShader->setUniformMat4("projection", m_projectionMatrix);
			m_renderShader->disable();

			init();
		}

		ForwardRenderer::ForwardRenderer(unsigned int width, unsigned int height, std::shared_ptr<Shader> renderShader, unsigned char antialiasingLevel)
			: m_antiAliasingLevel(antialiasingLevel), m_width(width), m_height(height), m_renderShader(renderShader), m_projectionMatrix(1.0f), m_lookatMatrix(1.0f), m_cameraPosition(1.0f, 1.0f, 1.0f)
		{

			float quadVertices[] = 
			{	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
				// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};

			m_targetFbo = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height));
			m_targetFbo->addColorAttachment(TextureFormat::RGB, TextureFormat::RGB, TextureDataType::UNSIGNED_BYTE);
			m_targetFbo->addDepthAttachment(TextureFormat::DEPTH);
			if (!m_targetFbo->isFrameBufferComplete())
			{
				LUMI_WARN("Watch out! Framebuffer target is not complete!");
			}
		

			m_targetVao = std::unique_ptr<VertexArray>(new VertexArray());

			m_targetVbo.submitData(&quadVertices[0], 24, VertexBufferType::Vec2);

			m_targetVao->addBuffer(m_targetVbo, BufferType::Positions, 4 * sizeof(float), (void*)(nullptr));
			m_targetVao->addBuffer(m_targetVbo, BufferType::Uvs, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			if (antialiasingLevel > 0)
			{
				m_multisampledFbo = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height, m_antiAliasingLevel * 2));
				m_multisampledFbo->addColorAttachment(TextureFormat::RGB, TextureFormat::RGB, TextureDataType::UNSIGNED_BYTE);

				m_multisampledFbo->addDepthAttachment(TextureFormat::DEPTH);
				if (!m_multisampledFbo->isFrameBufferComplete())
				{
					LUMI_WARN("Watch out! Framebuffer multisampled is not complete!");
				}

			}

			m_postPorcessingShader = std::shared_ptr<Shader>(ShaderFactory::PassthroughShader());

			m_renderShader->enable();
			m_renderShader->setUniform3f("lightPos", maths::vec3(0.0f, 0.0f, 35.0f));
			m_renderShader->setUniform3f("lightColor", maths::vec3(1.0f, 1.0f, 1.0f));
			m_renderShader->setUniform3f("objectColor", maths::vec3(0.4f, 0.5f, 1.9f));
			m_renderShader->setUniformMat4("projection", m_projectionMatrix);
			m_renderShader->disable();

			init();

		}

		ForwardRenderer::~ForwardRenderer()
		{
		}

		void ForwardRenderer::render(std::vector<Model>& renderList)
		{
			if (m_antiAliasingLevel > 0)
				m_multisampledFbo->bind();
			else
				m_targetFbo->bind();

			GlCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			GlCall(glEnable(GL_DEPTH_TEST));


			m_renderShader->enable();

			for (auto& model : renderList)
			{
				if (model.isMeshReady())
				{
					model.bind();

					if (model.isInstanced())
					{
						GlCall(glDrawElementsInstanced(GL_TRIANGLES, model.getMesh()->getIndexCount(), GL_UNSIGNED_INT, nullptr, model.getInstancedElementsCount()));
					}
					else
					{
						m_renderShader->setUniformMat4("model", model.getTransformationMatrix());

						GlCall(glDrawElements(GL_TRIANGLES, model.getMesh()->getIndexCount(), GL_UNSIGNED_INT, nullptr));

					}
				}
				else
				{
					model.getMesh();
					continue;
				}
			}

			if (m_antiAliasingLevel > 0)
			{
				m_multisampledFbo->blit(*m_targetFbo);
				m_multisampledFbo->unbind();
			}
			else
				m_targetFbo->unbind();


			m_postPorcessingShader->enable();
			m_targetVao->bind();

			GlCall(glDisable(GL_DEPTH_TEST));
			GlCall(glActiveTexture(GL_TEXTURE0));
			GlCall(glBindTexture(GL_TEXTURE_2D, m_targetFbo->getTextureId(0)));
			GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));

			GlCall(glEnable(GL_DEPTH_TEST));
		}

		void ForwardRenderer::setAntiAlisingLevel(unsigned char level)
		{
			if (m_antiAliasingLevel != level && level < 5)
			{
				m_antiAliasingLevel = level;

				if (m_antiAliasingLevel > 0)
				{
					m_multisampledFbo = std::unique_ptr<FrameBuffer>(new FrameBuffer(m_width, m_height, m_antiAliasingLevel * 2));
					m_multisampledFbo->addColorAttachment(TextureFormat::RGB, TextureFormat::RGB, TextureDataType::UNSIGNED_BYTE);
					m_multisampledFbo->addDepthAttachment(TextureFormat::DEPTH);
				}
			}
		}

		void ForwardRenderer::setPorjectionMatrix(const maths::mat4 & projectionMatrix)
		{
			m_projectionMatrix = projectionMatrix;

			m_renderShader->enable();
			m_renderShader->setUniformMat4("projection", m_projectionMatrix);
			m_renderShader->disable();
		}

		void ForwardRenderer::setCameraPosition(const maths::vec3 & cameraPosition)
		{
			m_cameraPosition = cameraPosition;

			m_renderShader->enable();
			m_renderShader->setUniform3f("viewPos", m_cameraPosition);
			m_renderShader->disable();
		}


		void ForwardRenderer::setCameraLookTarget(const maths::mat4 & lookatMatrix)
		{
			m_lookatMatrix = lookatMatrix;

			m_renderShader->enable();
			m_renderShader->setUniformMat4("view", m_lookatMatrix);
			m_renderShader->disable();
		}

		void ForwardRenderer::setRenderingTargetSize(unsigned int width, unsigned int height)
		{
			if (m_height != height || m_width != width)
			{
				m_height = height;
				m_width = width;

				m_targetFbo = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height));
				m_targetFbo->addColorAttachment(TextureFormat::RGB, TextureFormat::RGB, TextureDataType::UNSIGNED_BYTE);
				m_targetFbo->addDepthAttachment(TextureFormat::DEPTH);

				if (m_antiAliasingLevel > 0)
				{
					m_multisampledFbo = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height, m_antiAliasingLevel * 2));
					m_multisampledFbo->addColorAttachment(TextureFormat::RGB, TextureFormat::RGB, TextureDataType::UNSIGNED_BYTE);
					m_multisampledFbo->addDepthAttachment(TextureFormat::DEPTH);
				}

			}

		}

		void ForwardRenderer::init()
		{
			//GlCall(glClearDepth(0.0f));
			GlCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

			GlCall(glEnable(GL_BLEND));
			GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			GlCall(glEnable(GL_DEPTH_TEST));
			//GlCall(glDepthFunc(GL_GREATER));

			GlCall(glEnable(GL_CULL_FACE));
			GlCall(glCullFace(GL_BACK));
		}

	}
}

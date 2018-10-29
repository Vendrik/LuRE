#include "DeferredRenderer.h"

#include "..\utils\Log.h"
#include "..\utils\Debug.h"
#include "..\maths\math_utils.h"

namespace lumi {
	namespace graphics {

		DeferredRenderer::DeferredRenderer(unsigned int width, unsigned int height, std::shared_ptr<Shader> geometryShader, std::shared_ptr<Shader> lightningShader, std::shared_ptr<Shader> shadowShader)
			: m_width(width), m_height(height), m_geometryShader(geometryShader), m_lightingShader(lightningShader), m_shadowShader(shadowShader),
			m_projectionMatrix(1.0f), m_lookatMatrix(1.0f), m_cameraPosition(1.0f, 1.0f, 1.0f),
			m_lightsUbo(0)
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

			m_targetVao = std::unique_ptr<VertexArray>(new VertexArray());

			m_targetVbo.submitData(&quadVertices[0], 24, VertexBufferType::Vec2);

			m_targetVao->addBuffer(m_targetVbo, BufferType::Positions, 4 * sizeof(float), (void*)(nullptr));
			m_targetVao->addBuffer(m_targetVbo, BufferType::Uvs, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			m_GBuffer = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height));

			bool res = false;
			m_GBuffer->addColorAttachment(TextureFormat::RGBA16F, TextureFormat::RGB, TextureDataType::FLOAT); // Normals
			m_GBuffer->addColorAttachment(TextureFormat::RGBA16F, TextureFormat::RGBA, TextureDataType::FLOAT); // Colors + Specular intensity
			m_GBuffer->addDepthTexture();

			if (!m_GBuffer->isFrameBufferComplete())
			{
				LUMI_WARN("Watch out! Framebuffer target is not complete!");
			}

			m_shadowMapBuffer = std::unique_ptr<FrameBuffer>(new FrameBuffer(1024, 1024));
			m_shadowMapBuffer->addDepthTexture();
			m_shadowMapBuffer->resetDrawBuffer();
			m_shadowMapBuffer->resetReadBuffer();

			if (!m_shadowMapBuffer->isFrameBufferComplete())
			{
				LUMI_WARN("Watch out! Shadow map framebuffer target is not complete!");
			}

			m_geometryShader->enable();
			m_geometryShader->setUniform3f("objectColor", maths::vec3(0.4f, 0.5f, 1.9f));
			m_geometryShader->disable();

			m_lightingShader->enable();

			// Set uniform locations for texture sampler inside fragemnt shader
			m_lightingShader->setUniform1i("gNormal", 0);
			m_lightingShader->setUniform1i("gAlbedoSpec", 1);
			m_lightingShader->setUniform1i("gDepth", 2);
			m_lightingShader->setUniform1i("gShadowMap", 3);


			m_lightingShader->setUniform3f("lightColor", maths::vec3(1.0f, 1.0f, 1.0f));
			m_lightingShader->setUniform3f("lightPos", maths::vec3(0.0f, 0.0f, 35.0f));

			m_lightingShader->setUniformBlockBinding("Lights", m_lightsUbo.getBindingPoint());
			m_lightingShader->disable();

			init();
		}

		DeferredRenderer::~DeferredRenderer()
		{
		}

		void DeferredRenderer::render(std::vector<Model>& renderList)
		{
			// 1. Geometry pass

			m_GBuffer->bind();			
			m_geometryShader->enable();

			GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));		

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
						m_geometryShader->setUniformMat4("model", model.getTransformationMatrix());

						GlCall(glDrawElements(GL_TRIANGLES, model.getMesh()->getIndexCount(), GL_UNSIGNED_INT, nullptr));

					}
				}
				else
				{
					model.getMesh();
					continue;
				}
			}

			m_GBuffer->unbind();

			// 2. Lighting pass
			GlCall(glDisable(GL_DEPTH_TEST));
			m_lightingShader->enable();
			
			GlCall(glActiveTexture(GL_TEXTURE0));
			GlCall(glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureId(0)));
			GlCall(glActiveTexture(GL_TEXTURE1));
			GlCall(glBindTexture(GL_TEXTURE_2D, m_GBuffer->getTextureId(1)));
			GlCall(glActiveTexture(GL_TEXTURE2));
			GlCall(glBindTexture(GL_TEXTURE_2D, m_GBuffer->getDepthAttachmentId()));
			GlCall(glActiveTexture(GL_TEXTURE3));
			GlCall(glBindTexture(GL_TEXTURE_2D, m_shadowMapBuffer->getDepthAttachmentId()));

			// iterate over lights

			m_targetVao->bind();
			GlCall(glDrawArrays(GL_TRIANGLES, 0, 6));

			GlCall(glEnable(GL_DEPTH_TEST));
		}

		/*
		void DeferredRenderer::renderShadows(std::vector<Model>& renderList, const maths::vec3 & lightPosition)
		{

			glDisable(GL_BLEND);
			// 0. Shadow-Map pass

			maths::vec3 ligth_dir = lightPosition;
			ligth_dir.normalize();

			maths::mat4 shadow_view_matrix(1.0f); //= maths::mat4::translation(m_cameraPosition);
			//float pitch = acosf(maths::vec2(ligth_dir.x, ligth_dir.z).magnitude());
			//float yaw = maths::toDegrees(atanf(ligth_dir.x / ligth_dir.z));

			//yaw = ligth_dir.z > 0 ? yaw - 180 : yaw;

			//shadow_view_matrix *= maths::mat4::rotation(pitch, maths::vec3(1.0f, 0.0f, 0.0f));
			//shadow_view_matrix *= maths::mat4::rotation(yaw, maths::vec3(0.0f, 1.0f, 0.0f));

			//shadow_view_matrix *= maths::mat4::translation(m_cameraPosition * -1.0f);

			maths::vec3 shadow_right = maths::vec3::cross(ligth_dir, maths::vec3(0.0f, 1.0f, 0.0f)).normalize();
			maths::vec3 shadow_up = maths::vec3::cross(shadow_right, ligth_dir).normalize();

			shadow_view_matrix.elements[0] = shadow_right.x;
			shadow_view_matrix.elements[1] = shadow_up.x;
			shadow_view_matrix.elements[2] = ligth_dir.x;

			shadow_view_matrix.elements[4] = shadow_right.y;
			shadow_view_matrix.elements[5] = shadow_up.y;
			shadow_view_matrix.elements[6] = ligth_dir.y;

			shadow_view_matrix.elements[8] = shadow_right.z;
			shadow_view_matrix.elements[9] = shadow_up.z;
			shadow_view_matrix.elements[10] = ligth_dir.z;


			//maths::vec3 right = maths::vec3::cross(vec3(0.0f, 1.0f, 0.0f), ligth_dir);


			/*
			float tanFov = tanf(toRadians(0.5f * 45.0f));
			float aspectRatio = m_width / m_height;

			float hNear = 2 * tanFov * 1.0f;
			float wNear = hNear * aspectRatio;
			float hFar = 2 * tanFov * 200.0f;
			float wFar = hFar * aspectRatio;

			maths::vec3 cNear = m_cameraPosition + (m_cameraTarget - m_cameraPosition) * 1.0f;
			maths::vec3 cFar = m_cameraPosition + (m_cameraTarget - m_cameraPosition) * 200.0f;
			//Comment ends here

			maths::mat4 shadow_ortho = maths::mat4::ortographic(-20, 20, -40, 40, -10.0f, 200.0f);


			//create ortho matrix
			m_shadowShader->enable();

			m_shadowShader->setUniformMat4("lightView", shadow_view_matrix);
			m_shadowShader->setUniformMat4("projection", shadow_ortho);
			// set ortho matrix

			glViewport(0, 0, 1024, 1024);

			m_shadowMapBuffer->bind();

			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (auto& model : renderList)
			{
				if (!model.isMeshReady())
				{
					model.getMesh();
					continue;
				}

				model.bind();

				if (model.isInstanced())
				{
					glDrawElementsInstanced(GL_TRIANGLES, model.getMesh()->getIndexCount(), GL_UNSIGNED_INT, nullptr, model.getInstancedElementsCount());
				}
				else
				{
					m_shadowShader->setUniformMat4("model", model.getTransformationMatrix());

					glDrawElements(GL_TRIANGLES, model.getMesh()->getIndexCount(), GL_UNSIGNED_INT, nullptr);

				}

			}

			m_shadowMapBuffer->unbind();
			m_shadowShader->disable();

			glViewport(0, 0, m_width, m_height);

			m_lightingShader->enable();

			m_lightingShader->setUniformMat4("LigthSpaceMatrix", shadow_ortho * shadow_view_matrix);

			m_lightingShader->disable();

		}
		*/

		void DeferredRenderer::setPorjectionMatrix(const maths::mat4 & projectionMatrix)
		{
			m_projectionMatrix = projectionMatrix;

			m_geometryShader->enable();
			m_geometryShader->setUniformMat4("projection", m_projectionMatrix);
			m_geometryShader->disable();

			m_lightingShader->enable();
			m_lightingShader->setUniformMat4("PrjMatrix", m_projectionMatrix);
			m_lightingShader->setUniformMat4("InvPrjMatrix", m_projectionMatrix.inverted());
			m_lightingShader->disable();
		}

		void DeferredRenderer::setCameraPosition(const maths::vec3 & cameraPosition)
		{
			m_cameraPosition = cameraPosition;

			m_lightingShader->enable();
			m_lightingShader->setUniform3f("viewPos", m_cameraPosition);
			m_lightingShader->disable();
		}

		void DeferredRenderer::setCameraDirection(const maths::vec3 & cameraDirection)
		{
			m_lightingShader->enable();

			m_lightingShader->setUniform3f("cameraDir", cameraDirection);
			m_lightingShader->disable();

		}

		void DeferredRenderer::setViewportSettings(int viewport[4], float depthRange[2])
		{
			m_lightingShader->enable();

			m_lightingShader->setUniform4i("viewport", viewport);
			m_lightingShader->setUniform2f("depth_range", depthRange);
			m_lightingShader->disable();
		}

		void DeferredRenderer::updateViewportSettings()
		{
			int viewport[4];
			float depthRange[2];

			GlCall(glGetIntegerv(GL_VIEWPORT, viewport));
			GlCall(glGetFloatv(GL_DEPTH_RANGE, depthRange));

			m_lightingShader->enable();

			m_lightingShader->setUniform4i("viewport", viewport);
			m_lightingShader->setUniform2f("depth_range", depthRange);
			m_lightingShader->disable();

		}

		void DeferredRenderer::setCameraLookTarget(const maths::mat4 & lookatMatrix)
		{
			m_lookatMatrix = lookatMatrix;

			m_geometryShader->enable();
			m_geometryShader->setUniformMat4("view", m_lookatMatrix);
			m_geometryShader->disable();

			m_lightingShader->enable();
			m_lightingShader->setUniformMat4("ViewMatrix", m_lookatMatrix);
			m_lightingShader->disable();

		}

		void DeferredRenderer::setRenderingTargetSize(unsigned int width, unsigned int height)
		{
			if (m_height != height || m_width != width)
			{
				m_height = height;
				m_width = width;

				m_GBuffer = std::unique_ptr<FrameBuffer>(new FrameBuffer(width, height));
				m_GBuffer->addColorAttachment(TextureFormat::RGBA16F, TextureFormat::RGB, TextureDataType::FLOAT);
				m_GBuffer->addColorAttachment(TextureFormat::RGBA16F, TextureFormat::RGBA, TextureDataType::FLOAT);
				m_GBuffer->addDepthTexture();


				if (!m_GBuffer->isFrameBufferComplete())
				{
					LUMI_WARN("Watch out! Framebuffer target is not complete!");
				}

			}

		}
		void DeferredRenderer::setLights(std::vector<Light>& lights)
		{
			unsigned int lights_count = 32;

			// Hard coded 32 Lights cap, in accordance with shader
			if (lights.size() < 32)
				lights_count = lights.size();

			m_lightingShader->enable();

			void* ptr = m_lightsUbo.mapData(lights_count, sizeof(Light));
			memcpy(ptr, lights.data(), lights_count * sizeof(Light));
			m_lightsUbo.unmapData();

			m_lightingShader->setUniform1i("lights_count", lights_count);

			m_lightingShader->disable();

		}
		void DeferredRenderer::init()
		{
			GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			//GlCall(glClearDepth(0.0f));

			GlCall(glDisable(GL_BLEND));

			GlCall(glEnable(GL_DEPTH_TEST));
			//GlCall(glDepthFunc(GL_GREATER));

			GlCall(glEnable(GL_CULL_FACE));
			GlCall(glCullFace(GL_BACK));
		}
	}
}
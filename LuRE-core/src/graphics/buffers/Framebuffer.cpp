#include "Framebuffer.h"

#include "..\..\utils\Log.h"
#include "..\..\utils\Debug.h"

namespace lumi {
	namespace graphics {

		FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
			: m_id(0), m_width(width), m_height(height), m_samples(1), m_updateDrawTargets(true)
		{
			GlCall(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxAttachments));
			GlCall(glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_maxDrawBuffers));

			m_colorAttachments.reserve(m_maxAttachments);
			m_drawBufferTarget.reserve(m_maxDrawBuffers);

			GlCall(glGenFramebuffers(1, &m_id));

		}

		
		FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int samples)
			: m_id(0), m_width(width), m_height(height), m_samples(samples), m_updateDrawTargets(true)
		{
			GlCall(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxAttachments));
			GlCall(glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_maxDrawBuffers));

			m_colorAttachments.reserve(m_maxAttachments);
			m_drawBufferTarget.reserve(m_maxDrawBuffers);

			GlCall(glGenFramebuffers(1, &m_id));

		}

		FrameBuffer::FrameBuffer(FrameBuffer && fbo)
		{
			m_colorAttachments = std::move(fbo.m_colorAttachments);
			m_drawBufferTarget = std::move(fbo.m_drawBufferTarget);
			m_depthAttachmentId = fbo.m_depthAttachmentId;
			m_maxAttachments = fbo.m_maxAttachments;
			m_maxDrawBuffers = fbo.m_maxDrawBuffers;

			m_id = fbo.m_id;
			m_samples = fbo.m_samples;
			m_width = fbo.m_width;
			m_height = fbo.m_height;

			fbo.m_id = 0;
			fbo.m_width = 0;
			fbo.m_height = 0;
			fbo.m_depthAttachmentId = 0;
		}

		FrameBuffer::~FrameBuffer()
		{
			for(auto& elem : m_colorAttachments)
				GlCall(glDeleteTextures(1, &elem.id));
			
			GlCall(glDeleteRenderbuffers(1, &m_depthAttachmentId));
			GlCall(glDeleteFramebuffers(1, &m_id));
		}

		FrameBuffer & FrameBuffer::operator=(FrameBuffer && fbo)
		{
			if (this != &fbo) {
				this->~FrameBuffer();

				m_colorAttachments = std::move(fbo.m_colorAttachments);
				m_drawBufferTarget = std::move(fbo.m_drawBufferTarget);
				m_depthAttachmentId = fbo.m_depthAttachmentId;
				m_maxAttachments = fbo.m_maxAttachments;
				m_maxDrawBuffers = fbo.m_maxDrawBuffers;

				m_id = fbo.m_id;
				m_samples = fbo.m_samples;
				m_width = fbo.m_width;
				m_height = fbo.m_height;

				fbo.m_id = 0;
				fbo.m_width = 0;
				fbo.m_height = 0;
				fbo.m_depthAttachmentId = 0;

			}

			return *this;
		}

		bool FrameBuffer::addColorAttachment(TextureFormat internalFormat, TextureFormat incomingFormat, TextureDataType incomingType)
		{
			if (m_colorAttachments.size() >= m_maxAttachments)
			{
				LUMI_WARN("Watch out! Trying to attach more buffers than possible: %i", m_maxAttachments);
				return false;
			}

			unsigned int texture = 0;
			unsigned int attachment_position = GL_COLOR_ATTACHMENT0 + m_colorAttachments.size();

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
			GlCall(glGenTextures(1, &texture));

			if (m_samples > 1)
			{
				GlCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture));
				GlCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, internalFormat, m_width, m_height, GL_TRUE));
				GlCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));

				GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_position, GL_TEXTURE_2D_MULTISAMPLE, texture, 0));
			}
			else
			{
				GlCall(glBindTexture(GL_TEXTURE_2D, texture));
				GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				GlCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, incomingFormat, incomingType, nullptr));
				GlCall(glBindTexture(GL_TEXTURE_2D, 0));

				GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_position, GL_TEXTURE_2D, texture, 0));
			}

			m_drawBufferTarget.push_back(attachment_position);
			GlCall(glDrawBuffers(m_drawBufferTarget.size(), &m_drawBufferTarget[0]));

			m_colorAttachments.emplace_back(texture, m_width, m_height, m_samples, TextureFormat::NONE, AttachmentType::COLOR_TYPE);

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			return true;
		}

		void FrameBuffer::addDepthAttachment(TextureFormat format)
		{
			unsigned int rbo = 0;
			unsigned int gl_flag = 0;
			AttachmentType type;
			
			if (format == TextureFormat::DEPTH)
			{
				gl_flag = GL_DEPTH_ATTACHMENT;
				type = AttachmentType::DEPTH_TYPE;

			}
			else if (format == TextureFormat::DEPTH_24_STENCIL_8)
			{
				gl_flag = GL_DEPTH_STENCIL_ATTACHMENT;
				type = AttachmentType::DEPTH_AND_STENCIL_TYPE;
			}
			else
			{
				LUMI_ERROR("Depth attachment wrong format!");
				return;
			}

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
			GlCall(glGenRenderbuffers(1, &rbo));
			GlCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

			if (m_samples > 1)
				GlCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, format, m_width, m_height));

			else
				GlCall(glRenderbufferStorage(GL_RENDERBUFFER, format, m_width, m_height));

			GlCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

			GlCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, gl_flag, GL_RENDERBUFFER, rbo));
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

			m_depthAttachmentId = rbo;
		}

		void FrameBuffer::addDepthTexture()
		{
			unsigned int texture = 0;

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
			GlCall(glGenTextures(1, &texture));

			if (m_samples > 1)
			{
				GlCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture));
				GlCall(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH_COMPONENT32F, m_width, m_height, GL_TRUE));
				GlCall(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));

				GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, texture, 0));
			}
			else
			{
				GlCall(glBindTexture(GL_TEXTURE_2D, texture));
				GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
				GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
				GlCall(glBindTexture(GL_TEXTURE_2D, 0));
				
				GlCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0));
			}

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			m_depthAttachmentId = texture;
		}

		void FrameBuffer::bind()
		{
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

			/*if (m_updateDrawTargets)
			{
				unsigned int *buffers = (unsigned int *)alloca(sizeof(unsigned int) * m_colorAttachments.size());
				
				for (int i = 0; i < m_colorAttachments.size() && i < m_maxDrawBuffers; i++)
				{
					buffers[i] = GL_COLOR_ATTACHMENT0 + i;
				}

				glDrawBuffers(m_maxDrawBuffers, buffer);
				m_updateDrawTargets = false;
			}*/
		}

		void FrameBuffer::unbind()
		{
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

		void FrameBuffer::blit(FrameBuffer& target, bool withDepthBuffer)
		{
			if (m_colorAttachments.size() != target.m_colorAttachments.size())
			{
				LUMI_WARN("Watch out! You are trying to blit two fbo with different number of attachments!");
			}

			GlCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id));
			GlCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.m_id));
			GlCall(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, target.m_width, target.m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST));

			if (withDepthBuffer)
				GlCall(glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, target.m_width, target.m_height, GL_DEPTH_BUFFER_BIT , GL_NEAREST));
			
			GlCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
			GlCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));

		}

		unsigned int FrameBuffer::getTextureId(std::size_t index) const
		{
			if (index < m_colorAttachments.size()) 
				return m_colorAttachments[index].id;

			return 0;
		}

		bool FrameBuffer::isFrameBufferComplete()
		{
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

			bool res = false;
				
			GlCall(res = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			return res;
		}

		void FrameBuffer::resetReadBuffer()
		{
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

			glReadBuffer(GL_NONE);

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

		void FrameBuffer::resetDrawBuffer()
		{
			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

			glDrawBuffer(GL_NONE);

			GlCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

	}
}
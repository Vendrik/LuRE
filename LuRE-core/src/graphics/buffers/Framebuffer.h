#pragma once

#include <vector>
#include <GL\glew.h>

#include "..\TextureObject.h"

namespace lumi {
	namespace graphics {

		enum AttachmentType : unsigned char
		{
			NO_TYPE = 0,
			COLOR_TYPE,
			DEPTH_TYPE,
			DEPTH_AND_STENCIL_TYPE
		};

		struct Attachment
		{
			unsigned int id;
			unsigned int width;
			unsigned int height;
			unsigned int samples;
			TextureFormat format;
			AttachmentType type;

			Attachment() :
				id(0), width(0), height(0), samples(0), format(TextureFormat::NONE), type(AttachmentType::NO_TYPE) {};

			Attachment(unsigned int in_id, unsigned int in_width, unsigned int in_height, unsigned int in_samples, TextureFormat in_format, AttachmentType in_type) :
				id(in_id), width(in_width), height(in_height), samples(in_samples), format(in_format), type(in_type) {};

		};

		class FrameBuffer
		{

		private:

			std::vector<Attachment> m_colorAttachments;
			std::vector<unsigned int> m_drawBufferTarget;
			unsigned int m_depthAttachmentId;

			int m_maxAttachments;
			int m_maxDrawBuffers;

			unsigned int m_id;
			unsigned int m_samples;
			unsigned int m_width;
			unsigned int m_height;

			bool m_updateDrawTargets;

		public:
			FrameBuffer(unsigned int width, unsigned int height);
			FrameBuffer(unsigned int width, unsigned int height, unsigned int samples);
			FrameBuffer(FrameBuffer& fbo) = delete;
			FrameBuffer(FrameBuffer&& fbo);
			virtual ~FrameBuffer();

			FrameBuffer& operator=(FrameBuffer& fbo) = delete;
			FrameBuffer& operator=(FrameBuffer&& fbo);

			inline bool isMultisampled() const { return m_samples > 1; };
			inline unsigned int getNumberofSamples() const { return m_samples; };
			inline unsigned int getId() const { return m_id; };

			bool addColorAttachment(TextureFormat internalFormat, TextureFormat incomingFormat, TextureDataType incomingType);
			void addDepthAttachment(TextureFormat format);
			void addDepthTexture();
			
			unsigned int getTextureId(std::size_t index) const;
			inline unsigned int getDepthAttachmentId() const { return m_depthAttachmentId; };

			bool isFrameBufferComplete();
			void resetReadBuffer();
			void resetDrawBuffer();

			void bind();
			void unbind();

			void blit(FrameBuffer& target, bool withDepthBuffer = false);

		};

	}
}
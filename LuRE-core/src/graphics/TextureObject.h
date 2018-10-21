#pragma once

#include <GL\glew.h>
#include <string>

namespace lumi {
	namespace graphics {

		enum TextureType : unsigned int
		{
			TEXTURE_1D = GL_TEXTURE_1D,
			TEXTURE_2D = GL_TEXTURE_2D,
			TEXTURE_3D = GL_TEXTURE_3D,
			TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE
		};

		enum TextureFormat : unsigned int
		{
			NONE = 0,
			RED = GL_RED,
			RED32F = GL_R32F,
			RGB = GL_RGB,
			RGBA = GL_RGBA,
			RGB16 = GL_RGB16,
			RGBA16 = GL_RGBA16,
			RGB16F = GL_RGB16F,
			RGB32F = GL_RGB32F,
			RGBA16F = GL_RGBA16F,
			RGBA32F = GL_RGBA32F,
			DEPTH = GL_DEPTH_COMPONENT,
			DEPTH32 = GL_DEPTH_COMPONENT32,
			DEPTH_24_STENCIL_8 = GL_DEPTH24_STENCIL8

		};

		enum TextureDataType : unsigned int
		{
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			BYTE = GL_BYTE,
			UNSINGED_SHORT = GL_UNSIGNED_SHORT,
			SHORT = GL_SHORT,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			INT = GL_INT,
			FLOAT = GL_FLOAT
		};

		enum TextureFilteringMethod : unsigned int
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
		};

		class TextureObject
		{
		protected:
			std::string m_name;
			std::string m_fileName;

			unsigned int m_id;
			unsigned int m_bits;

			TextureType m_textureType;
			TextureFormat m_internalFormat;
			TextureFormat m_incomingDataFormat;
			TextureDataType m_incomingDataType;

			bool m_mipmapEnable;

		public:

			TextureObject();
			virtual ~TextureObject();

			void setFilters(TextureFilteringMethod minFilter, TextureFilteringMethod magFilter);

			void bind();
			void unbind();

			inline const unsigned int getTextureId() const { return m_id; };
			inline const TextureType getTextureType() const { return m_textureType; };
			inline const TextureFormat getTextureFormat() const { return m_internalFormat; };

		protected:
			virtual void initialize() = 0;
			virtual void initializeFromFile() = 0;

		};

	}
}
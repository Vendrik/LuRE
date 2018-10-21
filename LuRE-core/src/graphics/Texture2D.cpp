#include "Texture2D.h"

#include <FreeImage/FreeImage.h>
#include "../utils/ImageLoad.h"
#include "../utils/Log.h"


namespace lumi {
	namespace graphics {

		Texture2D::Texture2D(const std::string & fileName, TextureFormat internalFormat, TextureFormat incomingDataFormat, TextureDataType incomingDataType, bool mipmapEnable)
		{
			m_fileName = fileName;
			m_textureType = TextureType::TEXTURE_2D;
			m_internalFormat = internalFormat;
			m_incomingDataFormat = incomingDataFormat;
			m_incomingDataType = incomingDataType;

			m_mipmapEnable = mipmapEnable;

			initializeFromFile();
		}

		Texture2D::Texture2D(unsigned int width, unsigned int height, TextureFormat internalFormat, TextureFormat incomingDataFormat, TextureDataType incomingDataType)
		{
			m_width = width;
			m_height = height;

			m_textureType = TextureType::TEXTURE_2D;
			m_internalFormat = internalFormat;
			m_incomingDataFormat = incomingDataFormat;
			m_incomingDataType = incomingDataType;

			m_mipmapEnable = false;

			initialize();
		}

		Texture2D::Texture2D()
		{
		}

		void Texture2D::initialize()
		{
			glGenTextures(1, &m_id);
			glBindTexture(m_textureType, m_id);

			glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(m_textureType, 0, m_internalFormat, m_width, m_height, 0, m_incomingDataFormat, m_incomingDataType, nullptr);

			glBindTexture(m_textureType, 0);
		}

		void Texture2D::initializeFromFile()
		{
			LUMI_ASSERT(!m_fileName.empty(), "Texture file name can not be empty!");

			glGenTextures(1, &m_id);
			glBindTexture(m_textureType, m_id);

			unsigned char* pixels = load_image(m_fileName.c_str(), (GLsizei*)&m_width, (GLsizei*)&m_height, &m_bits);

			LUMI_ASSERT(pixels != nullptr, "Could not retrieve texture data from file");

			glTexImage2D(m_textureType, 0, m_internalFormat, m_width, m_height, 0, m_incomingDataFormat, m_incomingDataType, pixels);
			if (!m_mipmapEnable)
			{
				glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glGenerateMipmap(m_textureType);
				glTexParameteri(m_textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(m_textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
				

			delete[] pixels;

			glBindTexture(m_textureType, 0);
		}

		Texture2DMultisample::Texture2DMultisample(unsigned int width, unsigned int height, unsigned int samples, TextureFormat internalFormat, bool fixedSampleLocations)
		{
			LUMI_ASSERT(m_samples > 1, "Samples must be bigger than one!");

			m_width = width;
			m_height = height;
			m_samples = samples;

			m_textureType = TextureType::TEXTURE_2D_MULTISAMPLE;
			m_internalFormat = internalFormat;
			m_fixedSampleLocations = fixedSampleLocations;

			initialize();
		}

		void Texture2DMultisample::initialize()
		{
			glGenTextures(1, &m_id);
			glBindTexture(m_textureType, m_id);

			glTexImage2DMultisample(m_textureType, m_samples, m_internalFormat, m_width, m_height, m_fixedSampleLocations);

			glBindTexture(m_textureType, 0);
		}

		void Texture2DMultisample::initializeFromFile()
		{

		}

	}
}
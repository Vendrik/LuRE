#pragma once
#include "TextureObject.h"

namespace lumi {
	namespace graphics {

		class Texture2D : public TextureObject
		{
		protected:

			unsigned int m_width;
			unsigned int m_height;

		public:
			Texture2D(const std::string& fileName, TextureFormat internalFormat, TextureFormat incomingDataFormat, TextureDataType incomingDataType, bool mipmapEnable);
			Texture2D(unsigned int width, unsigned int height, TextureFormat internalFormat, TextureFormat incomingDataFormat, TextureDataType incomingDataType);

			inline const unsigned int getWidth() const { return m_width; };
			inline const unsigned int getHeight() const { return m_height; };

		protected:
			Texture2D();

			virtual void initialize() override;
			virtual void initializeFromFile() override;

		};


		class Texture2DMultisample : public Texture2D
		{
		protected:

			unsigned int m_samples;
			bool m_fixedSampleLocations;

		public:
			Texture2DMultisample(unsigned int width, unsigned int height, unsigned int samples, TextureFormat internalFormat, bool fixedSampleLocations = true);
			
			inline const unsigned int getSampleNumber() const { return m_samples; };

		protected:

			virtual void initialize() override;
			virtual void initializeFromFile() override;

		};

	}
}

#include "TextureObject.h"

namespace lumi { 
    namespace graphics {

		TextureObject::TextureObject()
		{
		}


		TextureObject::~TextureObject()
		{
		}

		void TextureObject::setFilters(TextureFilteringMethod minFilter, TextureFilteringMethod magFilter)
		{

		}

		void TextureObject::bind()
		{
			glBindTexture(m_textureType, m_id);
		}

		void TextureObject::unbind()
		{
			glBindTexture(m_textureType, 0);
		}

    }
}
#include "Light.h"

namespace lumi {
	namespace graphics {

		Light::Light() :
			m_position(0.0f, 0.0f, 0.0f, 1.0f), 
			m_color(1.0f, 1.0f, 1.0f, 1.0f)
		{

		}

		Light::Light(const maths::vec4& position, const maths::vec4& color) :
			m_position(position),
			m_color(color)
		{

		}

		Light::Light(const Light& light)
		{
			m_position = light.m_position;
			m_color = light.m_color;
		}

		Light::Light(Light&& light)
		{
			m_position = std::move(light.m_position);
			m_color = std::move(light.m_color);
		}

		Light& Light::operator=(Light& light)
		{
			if(this != &light)
			{
				m_position = light.m_position;
				m_color = light.m_color;
			}

			return *this;
		}

		Light& Light::operator=(Light&& light)
		{
			if (this != &light)
			{
				m_position = std::move(light.m_position);
				m_color = std::move(light.m_color);
			}

			return *this;
		}

		Light::~Light()
		{

		}

	}
}
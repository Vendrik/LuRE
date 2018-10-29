#pragma once

#include "..\maths\maths.h"

namespace lumi {
	namespace graphics {

		class Light
		{
		public:
			
			maths::vec4 m_position;
			maths::vec4 m_color;

		public:

			Light();
			Light(const maths::vec4& position, const maths::vec4& color);
			Light(const Light& light);
			Light(Light&& light);

			Light& operator=(Light& light);
			Light& operator=(Light&& light);

			~Light();

			//inline const maths::vec3& getPosition() const { return m_position; };
			//inline const maths::vec3& getColor() const { return m_color; };

		};

	}
}
#pragma once

#include <iostream>

namespace lumi {
	namespace maths {


		class vec4
		{

		public:

			float x, y, z, w;

		public:
			vec4();
			vec4(const float& x, const float& y, const float& z, const float& w);

			friend vec4 operator + (vec4 left, const vec4& right);
			friend vec4 operator - (vec4 left, const vec4& right);
			friend vec4 operator * (vec4 left, const vec4& right);
			friend vec4 operator / (vec4 left, const vec4& right);

			bool operator == (const vec4& other);
			bool operator != (const vec4& other);

			vec4& operator += (const vec4& other);
			vec4& operator -= (const vec4& other);
			vec4& operator *= (const vec4& other);
			vec4& operator /= (const vec4& other);

			friend std::ostream& operator << (std::ostream& stream, const vec4& vector);
		};

	}
}



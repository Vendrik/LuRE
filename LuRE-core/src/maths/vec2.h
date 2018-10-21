#pragma once

#include <iostream>

namespace lumi {
	namespace maths {

		class vec3;

		class vec2
		{

		public:

			float x, y;

		public:
			vec2();
			vec2(const float& x, const float& y);
			vec2(const vec3& vector);

			friend vec2 operator + (vec2 left, const vec2& right);
			friend vec2 operator - (vec2 left, const vec2& right);
			friend vec2 operator * (vec2 left, const vec2& right);
			friend vec2 operator / (vec2 left, const vec2& right);

			friend vec2 operator + (vec2 left, float value);
			friend vec2 operator * (vec2 left, float value);

			bool operator == (const vec2& other);
			bool operator != (const vec2& other);

			vec2& operator += (const vec2& other);
			vec2& operator -= (const vec2& other);
			vec2& operator *= (const vec2& other);
			vec2& operator /= (const vec2& other);

			float distance(const vec2& other) const;
			float dot(const vec2& other) const;
			float magnitude() const;
			vec2 normalize() const;

			friend std::ostream& operator << (std::ostream& stream, const vec2& vector);
		};

	}
}



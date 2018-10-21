#pragma once

#include <iostream>

namespace lumi {
	namespace maths {

		class vec2;

		class vec3
		{

		public:

			float x, y, z;

		public:
			vec3();
			vec3(float x, float y, float z);
			vec3(const vec2& vector);

			friend vec3 operator + (vec3 left, const vec3& right);
			friend vec3 operator - (vec3 left, const vec3& right);
			friend vec3 operator * (vec3 left, const vec3& right);
			friend vec3 operator / (vec3 left, const vec3& right);

			friend vec3 operator * (vec3 left, const float value);
			friend vec3 operator / (vec3 left, const float value);

			bool operator == (const vec3& other);
			bool operator != (const vec3& other);

			vec3& operator += (const vec3& other);
			vec3& operator -= (const vec3& other);
			vec3& operator *= (const vec3& other);
			vec3& operator /= (const vec3& other);

			float distance(const vec3 & other) const;
			static vec3 cross(const vec3& left, const vec3& right);
			static vec3 normalize(const vec3& other);

			vec3& normalize();

			friend std::ostream& operator << (std::ostream& stream, const vec3& vector);
		};

	}
}



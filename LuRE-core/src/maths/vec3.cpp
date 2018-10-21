#include "vec3.h"
#include "vec2.h"

namespace lumi {
	namespace maths {



		vec3::vec3()
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}

		vec3::vec3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vec3::vec3(const vec2& vector)
		{
			this->x = vector.x;
			this->y = vector.y;
			this->z = 0.0f;
		}

		vec3 operator + (vec3 left, const vec3& right)
		{
			left.x += right.x;
			left.y += right.y;
			left.z += right.z;

			return left;

		}

		vec3 operator - (vec3 left, const vec3& right)
		{
			left.x -= right.x;
			left.y -= right.y;
			left.z -= right.z;

			return left;
		}

		vec3 operator * (vec3 left, const vec3& right)
		{
			left.x *= right.x;
			left.y *= right.y;
			left.z *= right.z;

			return left;
		}

		vec3 operator / (vec3 left, const vec3& right)
		{
			left.x /= right.x;
			left.y /= right.y;
			left.z /= right.z;

			return left;
		}

		vec3 operator * (vec3 left, const float value)
		{
			left.x *= value;
			left.y *= value;
			left.z *= value;

			return left;
		}

		vec3 operator / (vec3 left, const float value)
		{
			left.x /= value;
			left.y /= value;
			left.z /= value;

			return left;
		}

		vec3& vec3::operator += (const vec3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;

			return *this;
		}

		vec3& vec3::operator -= (const vec3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;

			return *this;
		}

		vec3& vec3::operator *= (const vec3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;

			return *this;
		}

		vec3& vec3::operator /= (const vec3& other)
		{
			x /= other.x;
			y /= other.y;
			z /= other.z;

			return *this;
		}

		bool vec3::operator == (const vec3& other)
		{
			return x == other.x && y == other.y && z == other.z;
		}


		bool vec3::operator != (const vec3& other)
		{
			return !(*this == other);
		}

		float vec3::distance(const vec3 & other) const
		{
			float a = x - other.x;
			float b = y - other.y;
			float c = z - other.z;

			return sqrt(a * a + b * b + c * c);
		}

		vec3 vec3::cross(const vec3& left, const vec3& right)
		{
			return vec3(left.y*right.z - left.z*right.y, left.z*right.x - left.x*right.z, left.x*right.y - left.y*right.x);
		}

		vec3 vec3::normalize(const vec3& other)
		{
			float norm = sqrt(other.x * other.x + other.y * other.y + other.z * other.z);

			return vec3(other.x / norm, other.y / norm, other.z / norm);

		}

		vec3 & vec3::normalize()
		{
			float norm = sqrt(x * x + y * y + z * z);
			x /= norm;
			y /= norm;
			z /= norm;

			return *this;
		}

		std::ostream& operator << (std::ostream& stream, const vec3& vector) {

			return stream << "vec3: " << vector.x << ", " << vector.y << ", " << vector.z;
		}


	}
}

#include "vec2.h"
#include "vec3.h"

namespace lumi {
	namespace maths {



		vec2::vec2()
		{
			x = 0.0f;
			y = 0.0f;
		}

		vec2::vec2(const float& x, const float& y)
		{
			this->x = x;
			this->y = y;
		}

		vec2::vec2(const vec3& vector)
		{
			this->x = vector.x;
			this->y = vector.y;
		}

		vec2 operator + (vec2 left, const vec2& right)
		{
			left.x += right.x;
			left.y += right.y;

			return left;

		}
		
		vec2 operator - (vec2 left, const vec2& right)
		{
			left.x -= right.x;
			left.y -= right.y;

			return left;
		}
		
		vec2 operator * (vec2 left, const vec2& right)
		{
			left.x *= right.x;
			left.y *= right.y;

			return left ;
		}
		
		vec2 operator / (vec2 left, const vec2& right)
		{
			left.x /= right.x;
			left.y /= right.y;

			return left;
		}

		vec2 operator+(vec2 left, float value)
		{
			return vec2(left.x + value, left.y + value);
		}

		vec2 operator*(vec2 left, float value)
		{
			return vec2(left.x * value, left.y * value);
		}

		vec2& vec2::operator += (const vec2& other)
		{
			this->x += other.x;
			this->y += other.y;

			return *this;
		}

		vec2& vec2::operator -= (const vec2& other)
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		vec2& vec2::operator *= (const vec2& other)
		{
			x *= other.x;
			y *= other.y;

			return *this;
		}

		vec2& vec2::operator /= (const vec2& other)
		{
			x /= other.x;
			y /= other.y;

			return *this;
		}

		float vec2::distance(const vec2 & other) const
		{
			float a = x - other.x;
			float b = y - other.y;

			return sqrt(a * a + b * b);
		}

		float vec2::dot(const vec2 & other) const
		{
			return x * other.x + y * other.y;
		}

		float vec2::magnitude() const
		{
			return sqrt(x * x + y * y);
		}

		vec2 vec2::normalize() const
		{
			float len = magnitude();
			return vec2(x / len, y / len);
		}

		bool vec2::operator == (const vec2& other)
		{
			return x == other.x && y == other.y;
		}


		bool vec2::operator != (const vec2& other)
		{
			return !(*this == other);
		}

		std::ostream& operator << (std::ostream& stream, const vec2& vector) {

			return stream << "vec2: " << vector.x << ", " << vector.y;
		}


} }

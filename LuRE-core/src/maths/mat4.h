#pragma once

#include "maths.h"

namespace lumi {
	namespace maths {

		class mat4
		{

		public:

			union
			{
				float elements[4 * 4];
				vec4 columns[4];
			};

		public:
			mat4();
			mat4(float diagonal);
			mat4& invert();
			mat4  inverted() const;

			static mat4 identity();
			static mat4 ortographic(float left, float right, float bottom, float top, float near, float far);
			static mat4 perspective(float fov, float aspectRatio, float near, float far);
			static mat4 reversedZPerspective(float fov, float aspectRatio, float near);
			static mat4 rotation(float angle, const vec3& axis);
			static mat4 translation(const vec3& translation);
			static mat4 scale(const vec3& scale);
			static mat4 scale(const float scale);

			static mat4 transformation(const vec3& position, float rotX, float rotY, float rotZ, float scale);
			static mat4 transformation(const vec3& position, const vec3& rotation, float scale);
			static mat4 lookAt(const vec3& position, const vec3 & target, const vec3 & worldUp);

			mat4& operator *= (const mat4& other);
			//vec3 operator *= (const vec3& other) const;
			//vec4 operator *= (const vec4& other) const;
			
			friend mat4 operator * (mat4 left, const mat4& other);
			friend vec3 operator * (const mat4& left, const vec3& other);
			friend vec4 operator * (const mat4& left, const vec4& other);


		};


	}

}


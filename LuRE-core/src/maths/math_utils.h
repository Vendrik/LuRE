#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace lumi {
	namespace maths {

		inline float toRadians(float degrees) {
			return (float)(degrees * (M_PI / 180.0f));
		}

		inline float toDegrees(float radians) {
			return (float)(radians * (180.0f / M_PI));
		}

		inline int sign(float value) {
			return (value > 0) - (value < 0);
		}
	}
}
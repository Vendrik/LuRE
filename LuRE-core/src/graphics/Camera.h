#pragma once

#include "..\maths\maths.h"

namespace lumi {
	namespace graphics {

		enum CameraDirection {
			Forward,
			Backward,
			Left,
			Right
		};

		class Camera
		{
		private:

			maths::mat4 m_lookAt;

			maths::vec3 m_position;
			maths::vec3 m_worldUp;
			maths::vec3 m_up;
			maths::vec3 m_right;
			maths::vec3 m_direction;

			float m_cameraSpeed;
			float m_mouseSensitivity;
			float m_yaw;
			float m_pitch;
			bool m_toUpdateLookAt;

		public:
			Camera(const maths::vec3& position, const maths::vec3& worldUp, const float yaw, const float pitch, const float cameraSpeed);
			~Camera();

			const maths::mat4& getLookAt();
			inline maths::vec3 getTarget() const { return m_position + m_direction; };
			inline const maths::vec3& getPosition() const { return m_position; };
			inline const maths::vec3& getDirection() const { return m_direction; };
			inline float getYaw() const { return m_yaw; };
			inline float getPitch() const { return m_pitch; };

			void processKeyboard(CameraDirection direction, const float timeDelta);
			void processMouse(float dx, float dy);

		private:
			void updateCamera();
		};

	}
}

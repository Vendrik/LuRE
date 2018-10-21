#include "Camera.h"

#include "..\maths\math_utils.h"

namespace lumi {
	namespace graphics {

		Camera::Camera(const maths::vec3 & position, const maths::vec3 & worldUp, const float yaw, const float pitch, const float cameraSpeed)
			:m_position(position), m_worldUp(worldUp), m_yaw(yaw), m_pitch(pitch), m_cameraSpeed(cameraSpeed), m_mouseSensitivity(0.4f), m_toUpdateLookAt(true)
		{
			updateCamera();
		}

		Camera::~Camera()
		{
		}

		const maths::mat4& Camera::getLookAt()
		{
			if (m_toUpdateLookAt) {
				m_lookAt = maths::mat4::lookAt(m_position, m_position + m_direction, m_up);
				m_toUpdateLookAt = false;
			}

			return m_lookAt;
		}

		/*
		const maths::mat4 & Camera::getTarget(const maths::vec3& target)
		{
			if (m_toUpdateLookAt) {
				m_lookAt = maths::mat4::lookAt(m_position, m_position + target, m_up);
				m_toUpdateLookAt = false;
			}

			return m_lookAt;
		}
		*/

		void Camera::processKeyboard(CameraDirection direction, const float timeDelta)
		{
			float velocity = m_cameraSpeed * timeDelta;

			if (direction == Forward) {
				m_position += m_direction * velocity;
			}
			else if (direction == Backward) {
				m_position -= m_direction * velocity;
			}
			else if (direction == Right) {
				m_position += m_right * velocity;
			}
			else {
				m_position -= m_right * velocity;
			}

			m_toUpdateLookAt = true;

		}

		void Camera::processMouse(float dx, float dy)
		{
			if (dx == 0 && dy == 0)
				return;

			dx *= m_mouseSensitivity;
			dy *= m_mouseSensitivity;

			m_yaw += dx;
			m_pitch += dy;

			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			else if (m_pitch < -89.0f)
				m_pitch = -89.0f;

			m_toUpdateLookAt = true;
			updateCamera();

		}

		void Camera::updateCamera()
		{

			m_direction.x = cos(maths::toRadians(m_yaw)) * cos(maths::toRadians(m_pitch));
			m_direction.y = sin(maths::toRadians(m_pitch));
			m_direction.z = sin(maths::toRadians(m_yaw)) * cos(maths::toRadians(m_pitch));

			m_direction.normalize();

			m_right = maths::vec3::cross(m_direction, m_worldUp).normalize(); 
			m_up = maths::vec3::cross(m_right, m_direction).normalize();

		}

	}
}

// ¯\_(ツ)_/¯
#include "stdafx.h"

#include "Camera.h"
#include "Scene.h"

namespace MyGL
{
	Camera::Camera(float speed, float mouseSensitivity)
		: _speed(speed)
		, _mouseSensitivity(mouseSensitivity)
	{
		RecalcQuat();
	}

	Camera::~Camera() = default;

	void Camera::SetPosition(glm::vec3 pos)
	{
		_position = pos;
	}

	void Camera::Move(MoveDirection moveDirection, float dt)
	{
		glm::vec3 dir(0.f);
		switch (moveDirection) {
		case MoveDirection::Left: dir = glm::vec3(-1.f, 0.f, 0.f); break;
		case MoveDirection::Right: dir = glm::vec3(1.f, 0.f, 0.f); break;
		case MoveDirection::Down: dir = glm::vec3(0.f, -1.f, 0.f); break;
		case MoveDirection::Up: dir = glm::vec3(0.f, 1.f, 0.f); break;
		case MoveDirection::Backward: dir = glm::vec3(0, 0.f, 1.f); break;
		case MoveDirection::Forward: dir = glm::vec3(0, 0.f, -1.f); break;
		}

		dir = _rotation * dir;
		_position += dir * (_speed * dt);
	}

	void Camera::MouseMoved(float dx, float dy)
	{
		_pitch -= dy * _mouseSensitivity;
		_yaw -= dx * _mouseSensitivity;

		_pitch = glm::mod(_pitch, glm::two_pi<float>());
		_yaw = glm::mod(_yaw, glm::two_pi<float>());

		RecalcQuat();
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		glm::mat4 mat = glm::mat4_cast(glm::inverse(_rotation)) * glm::translate(glm::mat4(1.f), -_position);
		return mat;
	}

	glm::vec3 Camera::GetPosition() const
	{
		return _position;
	}

	void Camera::RecalcQuat()
	{
		_rotation = glm::angleAxis(_yaw, glm::vec3(0.f, 1.f, 0.f)) * glm::angleAxis(_pitch, glm::vec3(1.f, 0.f, 0.f));
	}
}

// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{

	class Camera
	{
	public:
		enum class MoveDirection
		{
			Left, Right,
			Up, Down,
			Backward, Forward
		};

		Camera(float speed = 12.f, float mouseSensitivity = 0.0015f);
		~Camera(); 

		void SetPosition(glm::vec3 pos);

		void Move(MoveDirection moveDirection, float dt);
		void MouseMoved(float dx, float dy);

		glm::mat4 GetViewMatrix() const;
		glm::vec3 GetPosition() const;
		glm::vec3 GetDirection() const;

	private:
		void RecalcQuat();

	private:
		float _speed = 1.f;
		float _mouseSensitivity = 1.f;

		float _yaw = 0;
		float _pitch = 0;
		glm::vec3 _position = glm::vec3(0.f);

		glm::quat _rotation = glm::angleAxis(0.f, glm::vec3(1.f, 0.f, 0.f));
	};
}

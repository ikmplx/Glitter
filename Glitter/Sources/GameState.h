// ¯\_(ツ)_/¯

#pragma once

#include "Forward.h"

namespace MyGL
{
	class GameState
	{
	public:
		GameState();
		virtual ~GameState();

		virtual void Update(float dt);
		virtual void Draw();

		void SetViewportSize(int width, int height);

		Camera& GetCamera();

	protected:
		std::unique_ptr<Camera> _camera;
		int _vpWidth = 0, _vpHeight = 0;
	};
}

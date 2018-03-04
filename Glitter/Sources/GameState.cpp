// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "GameState.h"
#include "Scene/Camera.h"

namespace MyGL
{
	GameState::GameState()
		: _camera(std::make_unique<Camera>())
	{
		glGenVertexArrays(1, &_emptyVao);
	}

	GameState::~GameState()
	{
		glDeleteVertexArrays(1, &_emptyVao);
	}

	void GameState::Update(float dt)
	{
	}

	void GameState::Draw()
	{
	}

	void GameState::SetViewportSize(int width, int height)
	{
		_vpWidth = width;
		_vpHeight = height;
	}

	Camera& GameState::GetCamera()
	{
		return *_camera;
	}
}

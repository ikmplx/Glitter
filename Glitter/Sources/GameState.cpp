// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "GameState.h"
#include "Scene/Camera.h"

namespace MyGL
{
	GameState::GameState()
		: _camera(std::make_unique<Camera>())
	{
	}

	GameState::~GameState()
	{
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

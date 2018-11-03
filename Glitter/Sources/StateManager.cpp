// ¯\_(ツ)_/¯

#include "stdafx.h"

#include "StateManager.h"

namespace MyGL
{
	StateManager::StateManager()
	{
		SetupStandardPresets();
	}

	StateManager::~StateManager() = default;

	void StateManager::SetState(StateType stateType)
	{
		SetState(_presets[(int) stateType]);
	}

	void StateManager::SetupStandardPresets()
	{
		AddPreset(StateType::Empty, [](State& state) {
			state.depthFunc = GL_LESS;
			state.depthTest = false;
			state.depthWrite = false;
			state.blend = false;
		});

		AddPreset(StateType::Perspective, [](State& state) {
			state.depthFunc = GL_LESS;
			state.depthTest = true;
			state.depthWrite = true;
			state.blend = false;
		});

		AddPreset(StateType::OpaqueSprite, [](State& state) {
			state.depthTest = false;
			// state.depthWrite = false; // don't used, when depth test disabled
			state.blend = false;
		});

		AddPreset(StateType::LightPass, [](State& state) {
			state.depthTest = false;
			state.blend = true;
			state.blendFunc = std::make_pair(GL_ONE, GL_ONE);
		});

		AddPreset(StateType::Skybox, [](State& state) {
			state.depthFunc = GL_LEQUAL; // TODO: Not sure, why we need LEQUAL here
			state.depthTest = true;
			state.depthWrite = false;
			state.blend = false;
		});
	}

	void StateManager::SetState(const State& state)
	{
		TryChangeValue(state.depthTest, _activeState.depthTest, [](bool depthTest) {
			GlEnable(GL_DEPTH_TEST, depthTest);
		});

		TryChangeValue(state.depthWrite, _activeState.depthWrite, [](bool depthWrite) {
			glDepthMask(depthWrite);
		});


		TryChangeValue(state.depthFunc, _activeState.depthFunc, [](GLenum depthFunc) {
			glDepthFunc(depthFunc);
		});

		TryChangeValue(state.blend, _activeState.blend, [](bool blend) {
			GlEnable(GL_BLEND, blend);
		});

		TryChangeValue(state.blendFunc, _activeState.blendFunc, [](const auto& pair) {
			glBlendFunc(pair.first, pair.second);
		});
	}

	StateManager* StateManager::Instance()
	{
		static StateManager stateManager;
		return& stateManager;
	}

	void StateManager::GlEnable(GLenum key, bool state)
	{
		if (state) {
			glEnable(key);
		} else {
			glDisable(key);
		}
	}
}

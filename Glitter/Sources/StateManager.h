// ¯\_(ツ)_/¯

#pragma once

namespace MyGL
{
	struct State
	{
		std::optional<bool> depthTest;
		std::optional<bool> depthWrite;
		std::optional<GLenum> depthFunc;

		std::optional<bool> blend;
	};

	enum class StateType
	{
		Empty,
		Perspective,
		ScreenQuad,
		Skybox,

		CountStateTypes
	};

	// OpenGL state manager, not related to GameState
	class StateManager
	{
	public:
		StateManager();
		~StateManager();

		void SetState(StateType stateType);
		void SetState(const State& state);

		static StateManager* Instance();

	private:
		template<typename Fun>
		void AddPreset(StateType stateType, Fun fun)
		{
			_presets[(int) stateType] = State();
			fun(_presets[(int) stateType]);
		}

		template<typename T, typename Fun>
		static void TryChangeValue(const std::optional<T>& src, std::optional<T>& dst, Fun fun)
		{
			if (src.has_value() && src != dst) {
				fun(src.value());
				dst = src;
			}
		}

		void SetupStandardPresets();

		static void GlEnable(GLenum key, bool state);

	private:
		State _activeState;

		std::array<State, (int) StateType::CountStateTypes> _presets;
	};
}

#pragma once

#include "Forward.h"

namespace MyGL
{
	struct ModelPrivate;
	class Model
	{
	public:
		Model(const std::string& path);
		~Model();

		void Draw(ShaderPtr shader);

	private:
		std::unique_ptr<ModelPrivate> d;
	};

	using ModelPtr = std::shared_ptr<Model>;
}
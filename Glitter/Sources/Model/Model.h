#pragma once

#include "Forward.h"

namespace MyGL
{
	class ModelLoaderPrivate;

	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader();

		EntityPtr Load(const std::string& path);
		static EntityPtr LoadModel(const std::string& path);

		void SetTransform(const glm::mat4& transform);

	private:
		std::unique_ptr<ModelLoaderPrivate> d;
	};
}
#pragma once

#include "Forward.h"

namespace MyGL
{
	class ModelLoaderPrivate;

	class ModelLoader
	{
	public:
		ModelLoader(const std::string& path);

		EntityPtr Load();
		static EntityPtr LoadModel(const std::string& path);

	private:
		std::unique_ptr<ModelLoaderPrivate> d;
	};
}
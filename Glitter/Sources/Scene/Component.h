// ¯\_(ツ)_/¯

#pragma once

#include <typeindex>

#include <Forward.h>

namespace MyGL
{
	struct Component
	{
		Component();
		virtual ~Component();

		EntityWeakPtr entity;
	};

#if false
	class ComponentRepo
	{
		struct Key
		{
			std::type_index componentType;
			unsigned entityId;
		};

		struct KeyHasher {
			size_t operator()(const Key& key);
		};

	public:
		ComponentRepo();
		~ComponentRepo();

		ComponentRepo* Instance();

		unsigned GenEntityId();

		void StoreComponent(unsigned entityId, ComponentPtr component);
		ComponentPtr FindComponent(unsigned entityId, const std::type_index& type);

	private:
		unsigned entityIdCtr = 0;
		std::unordered_map<Key, ComponentWeakPtr, KeyHasher> _repo;
	};
#endif
}

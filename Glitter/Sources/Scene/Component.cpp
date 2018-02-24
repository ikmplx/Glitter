// ¯\_(ツ)_/¯
#include "stdafx.h"

#include "Component.h"
#include "Utils.h"

namespace MyGL
{
	Component::Component()
	{
	}

	Component::~Component()
	{
	}

#if false
	size_t ComponentRepo::KeyHasher::operator()(const Key& key)
	{
		return Utils::HashCombine(key.componentType, key.entityId);
	}



	ComponentRepo* ComponentRepo::Instance()
	{
		static ComponentRepo instance;
		return &instance;
	}

	unsigned ComponentRepo::GenEntityId()
	{
		return ++entityIdCtr;
	}

	void ComponentRepo::StoreComponent(unsigned entityId, ComponentPtr component)
	{
		_repo[Key{ typeid(*component), entityId }] = component;
	}

	ComponentPtr ComponentRepo::FindComponent(unsigned entityId, const std::type_index & type)
	{
		auto iter = _repo.find(Key{ type, entityId });
		if (iter != _repo.end()) {
			return iter->second.lock();
		}
		else {
			return nullptr;
		}
	}
#endif
}

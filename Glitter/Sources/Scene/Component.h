// ¯\_(ツ)_/¯

#pragma once

#include <typeindex>

#include <Forward.h>
#include "Utils.h"

namespace MyGL
{
	struct Component
	{
		Component();
		virtual ~Component();

		virtual int GetTypeId() const = 0;
	};

	template<typename T>
	struct TypedComponent : Component
	{
		virtual int GetTypeId() const override
		{
			return Utils::TypeId<T, Component>::GetId();
		}

		static int GetTypeIdStatic()
		{
			return Utils::TypeId<T, Component>::GetId();
		}
	};
}

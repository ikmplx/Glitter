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

		virtual int GetTypeId() const = 0;

		static int sComponentTypeCounter;
	};

	template<typename T>
	struct TypedComponent : Component
	{
		virtual int GetTypeId() const override
		{
			return GetTypeIdStatic();
		}

		static int GetTypeIdStatic()
		{
			static int myType = sComponentTypeCounter++;
			return myType;
		}
	};
}

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

		int componentTypeId = -1;
	};
}

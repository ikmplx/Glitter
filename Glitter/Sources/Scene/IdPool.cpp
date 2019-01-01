// ¯\_(ツ)_/¯
#include "stdafx.h"

#include "IdPool.h"
#include "Utils.h"

namespace MyGL
{
	IdPool::IdPool()
	{
		_allIds.resize(1, false);
		_pool.push_back(0);
	}

	IdPool::~IdPool()
	{
	}

	int IdPool::Alloc()
	{
		TryGrowPool();

		int id = _pool.back();
		_pool.pop_back();

		MyAssert(!_allIds[id]);

		_allIds[id] = true;
		return id;
	}

	void IdPool::Dealloc(int id)
	{
		MyAssert(_allIds[id]);
		_allIds[id] = false;
		_pool.push_back(id);
	}

	void IdPool::TryGrowPool()
	{
		if (_pool.empty()) {
			int lastId = (int) _allIds.size() - 1;

			size_t newIdsCount = _allIds.size();
			_allIds.resize(newIdsCount + _allIds.size(), false);

			for (size_t i = 0; i < newIdsCount; i++) {
				_pool.push_back(lastId + (newIdsCount - i));
			}
		}
	}
}

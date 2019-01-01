// ¯\_(ツ)_/¯

#pragma once

namespace MyGL
{
	class IdPool
	{
	public:
		IdPool();
		~IdPool();

		int Alloc();
		void Dealloc(int id);

	private:
		void TryGrowPool();

	private:
		std::vector<int> _pool;
		std::vector<bool> _allIds;
	};
}
